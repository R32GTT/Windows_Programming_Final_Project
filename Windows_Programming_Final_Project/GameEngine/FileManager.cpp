#include "pch.h"
#include <fstream>
#include "FileManager.h"
#include "FileBase/FileBase.h"
#include "FileBase/FileTypes/Texture.h"
#include "FileBase/FileTypes/Sprite.h"
#include "FileBase/FileTypes/FlipBook.h"

using json = nlohmann::json;

static std::wstring StrToWStr(const std::string& str) {
	if (str.empty()) return L"";
	int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring result(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size);
	return result;
}




FileManager::~FileManager()
{
	Clear();
}

void FileManager::Init(HWND hwnd, fs::path filepath, ID2D1RenderTarget* renderTarget, IWICImagingFactory* wicFactory)
{
	hWnd = hwnd;
	filePath = filepath;
	_renderTarget = renderTarget;
	_wicFactory = wicFactory;
}

void FileManager::Clear()
{
	for (auto& item : _textures)
		SAFE_DELETE(item.second);

	_textures.clear();

	for (auto& item : _sprites)
		SAFE_DELETE(item.second);

	_sprites.clear();

	for (auto& item : _flipbooks)
		SAFE_DELETE(item.second);

	_flipbooks.clear();
}

void FileManager::LoadAespriteJson(const std::wstring& path)
{

	fs::path fullPath = filePath / path;
	std::ifstream file(fullPath);

	if (!file.is_open()) {
		::MessageBox(hWnd, fullPath.c_str(), L"JSON 파싱 실패: 파일이 없습니다.", MB_OK);
		return;
	}

	json j;
	file >> j; // JSON 파일 파싱 완료

	// 1. 메타 데이터에서 원본 이미지(Texture) 로드
	std::string imgNameStr = j["meta"]["image"];
	std::wstring imgName = StrToWStr(imgNameStr);

	fs::path imagePath = fullPath.parent_path() / imgName;
	std::wstring texKey = fullPath.stem().wstring(); // 예: Player.json -> "Player"

	Texture* tex = LoadTexture(texKey, imagePath.wstring());

	// 2. Frames 배열을 순회하며 Sprite들 조각내기
	std::vector<Sprite*> allSprites;
	for (auto& [frameName, frameData] : j["frames"].items())
	{
		float x = frameData["frame"]["x"];
		float y = frameData["frame"]["y"];
		float w = frameData["frame"]["w"];
		float h = frameData["frame"]["h"];

		std::wstring sprKey = StrToWStr(frameName);

		Sprite* sprite = CreateSprite(sprKey, tex, x, y, w, h);
		allSprites.push_back(sprite);
	}

	// 3. FrameTags를 읽어서 애니메이션(FlipBook) 상태별로 묶어주기
	if (j["meta"].contains("frameTags"))
	{
		for (auto& tag : j["meta"]["frameTags"])
		{
			std::wstring animName = StrToWStr(tag["name"]);
			int from = tag["from"];
			int to = tag["to"];

			// 엔진 안에서 꺼내 쓸 키값 생성 (예: "Player_Idle")
			std::wstring fbKey = texKey + L"_" + animName;
			FlipBook* fb = CreateFlipBook(fbKey);

			FlipbookInfo info;
			info.name = fbKey;

			// from ~ to 인덱스의 스프라이트들을 배열에 밀어넣음
			for (int i = from; i <= to; ++i) {
				info.frames.push_back(allSprites[i]);
			}

			// Aseprite의 첫 프레임 재생 시간을 엔진용 초 단위로 변환
			int durationMs = j["frames"].begin().value()["duration"];
			info.duration = durationMs / 1000.0f;

			fb->SetInfo(info);
		}
	}
}

Texture* FileManager::LoadTexture(const std::wstring& key, const std::wstring& path)
{
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	fs::path fullPath = filePath / path;

	Texture* texture = new Texture();
	texture->Load(_renderTarget, _wicFactory, fullPath.wstring());
	_textures[key] = texture;

	return texture;
}

Sprite* FileManager::CreateSprite(const std::wstring& key, Texture* texture, float x, float y, float cx, float cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	if (cx == 0.f)
		cx = texture->GetSize().x;

	if (cy == 0.f)
		cy = texture->GetSize().y;

	Sprite* sprite = new Sprite(texture, x, y, cx, cy);
	_sprites[key] = sprite;

	return sprite;
}

FlipBook* FileManager::CreateFlipBook(const std::wstring& key)
{
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	FlipBook* fb = new FlipBook();
	_flipbooks[key] = fb;

	return fb;
}
