#pragma once
#include "../GameEngine/FileBase/json.hpp"


class FileBase;
class Texture;
class Sprite;
class FlipBook;

class FileManager
{
public:

	DECLARE_SINGLE(FileManager);

	~FileManager();

public:
	void Init(HWND hwnd, fs::path filepath, ID2D1RenderTarget* renderTarget, IWICImagingFactory* wicFactory);
	
	void Clear();

	const fs::path& GetFilePath() { return filePath; };

	
	void LoadAespriteJson(const std::wstring& path);
	void LoadGlobalResources();

	Texture* GetTexture(const std::wstring& key) { return _textures[key]; };
	Texture* LoadTexture(const std::wstring& key, const std::wstring& path);

	Sprite* GetSprite(const std::wstring& key) { return _sprites[key]; };
	Sprite* CreateSprite(const std::wstring& key, Texture* texture, float x = 0, float y = 0, float cx = 0, float cy = 0);

	FlipBook* GetFlipBook(const std::wstring& key) { return _flipbooks[key]; };
	FlipBook* CreateFlipBook(const std::wstring& key);



private:
	HWND hWnd{};
	fs::path filePath{};

	ID2D1RenderTarget* _renderTarget{};
	IWICImagingFactory* _wicFactory{};
	
	std::unordered_map<std::wstring, Texture*>		_textures;
	std::unordered_map<std::wstring, Sprite*>		_sprites;
	std::unordered_map<std::wstring, FlipBook*>		_flipbooks;

	//std::unordered_map<std::wstring, Texture*> TODO SOUND
};

