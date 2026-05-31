#include "pch.h"
#include "FileManager.h"
#include "FileBase/FileBase.h"
#include "FileBase/FileTypes/Texture.h"
#include "FileBase/FileTypes/Sprite.h"
#include "FileBase/FileTypes/FlipBook.h"

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

Sprite* FileManager::CreateSprite(const std::wstring& key, Texture* texture, int x, int y, int cx, int cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	if (cx == 0)
		cx = texture->GetSize().x;

	if (cy == 0)
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
