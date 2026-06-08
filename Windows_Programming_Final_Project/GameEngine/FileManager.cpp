#include "pch.h"
#include <fstream>
#include "FileManager.h"
#include "FileBase/FileBase.h"
#include "FileBase/FileTypes/Texture.h"
#include "FileBase/FileTypes/Sprite.h"
#include "FileBase/FileTypes/FlipBook.h"

using json = nlohmann::json;


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
		::MessageBox(hWnd, fullPath.c_str(), L"파일이 없습니다.", MB_OK);
		return;
	}

	json j;
	file >> j;


	std::string imgNameStr = j["meta"]["image"];
	std::wstring imgName = StrToWStr(imgNameStr);

	fs::path relativePath = path;
	fs::path imageRelativePath = relativePath.parent_path() / imgName;

	std::wstring texKey = fullPath.stem().wstring();

	Texture* tex = LoadTexture(texKey, imageRelativePath.wstring());

	std::vector<Sprite*> allSprites;
	std::vector<int> allDurations; 

	if (j["frames"].is_array())
	{
		for (auto& frameData : j["frames"])
		{
			std::wstring sprKey = StrToWStr(frameData["filename"]);
			float x = frameData["frame"]["x"];
			float y = frameData["frame"]["y"];
			float w = frameData["frame"]["w"];
			float h = frameData["frame"]["h"];

			allSprites.push_back(CreateSprite(sprKey, tex, x, y, w, h));
			allDurations.push_back(frameData["duration"]);
		}
	}
	else if (j["frames"].is_object())
	{
		for (auto it = j["frames"].begin(); it != j["frames"].end(); ++it)
		{
			auto& frameData = it.value();
			std::wstring sprKey = StrToWStr(frameData["filename"]);
			float x = frameData["frame"]["x"];
			float y = frameData["frame"]["y"];
			float w = frameData["frame"]["w"];
			float h = frameData["frame"]["h"];

			allSprites.push_back(CreateSprite(sprKey, tex, x, y, w, h));
			allDurations.push_back(frameData["duration"]);
		}
	}


	if (j["meta"].contains("frameTags"))
	{
		for (auto& tag : j["meta"]["frameTags"])
		{

			std::wstring animName = StrToWStr(tag["name"]);
			int from = tag["from"];
			int to = tag["to"];


			std::wstring fbKey = texKey + L"_" + animName;
			FlipBook* fb = CreateFlipBook(fbKey);

			FlipbookInfo info;
			info.name = fbKey;


			if (from >= 0 && from < allDurations.size()) {
				info.duration = allDurations[from] / 1000.0f;
			}


			for (int i = from; i <= to; ++i) {
				info.frames.push_back(allSprites[i]);
			}

			fb->SetInfo(info);
		}
	}
}

void FileManager::LoadGlobalResources()
{
	LoadAespriteJson(L"MainCharAnim.json");
	LoadAespriteJson(L"MobCharAnim.json");
	LoadAespriteJson(L"FMobCharAnim.json");
	LoadAespriteJson(L"Deco1CharAnim.json");
	LoadAespriteJson(L"Deco2CharAnim.json");
	LoadAespriteJson(L"GunCharAnim.json");
	LoadAespriteJson(L"CrowCharAnim.json");
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

void FileManager::SaveMapJson(const std::wstring& fileName, const json& mapJson)
{
	fs::path fullPath = filePath / fileName;

	std::ofstream file(fullPath);

	if (file.is_open())
	{
		file << mapJson.dump(4);
		file.close();
	}
	else
	{
		::MessageBox(hWnd, fullPath.c_str(), L"맵 파일 저장에 실패했습니다.", MB_OK);
	}
}

json FileManager::LoadMapJson(const std::wstring& fileName)
{
	
	fs::path fullPath = filePath / fileName;

	std::ifstream file(fullPath);

	if (!file.is_open())
	{
		return json{};
	}

	json mapJson;
	try
	{
		file >> mapJson;
	}
	catch (json::parse_error& e)
	{
		file.close();
		return json{};
	}

	file.close();
	return mapJson;
	
}
