#pragma once

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
	void Init(HWND hwnd, fs::path filepath);
	
	void Clear();

	const fs::path& GetFilePath() { return filePath; };

	Texture* GetTexture(const std::wstring& key) { return _textures[key]; };
	Texture* LoadTexture(const std::wstring& key, const std::wstring& path, unsigned int transparent = RGB(255, 0, 255));

	Sprite* GetSprite(const std::wstring& key) { return _sprites[key]; };
	Sprite* CreateSprite(const std::wstring& key, Texture* texture, int x = 0, int y = 0, int cx = 0, int cy = 0);

	FlipBook* GetFlipBook(const std::wstring& key) { return _flipbooks[key]; };
	FlipBook* CreateFlipBook(const std::wstring& key);


private:
	HWND hWnd{};
	fs::path filePath{};
	
	std::unordered_map<std::wstring, Texture*>		_textures;
	std::unordered_map<std::wstring, Sprite*>		_sprites;
	std::unordered_map<std::wstring, FlipBook*>		_flipbooks;

	//std::unordered_map<std::wstring, Texture*> TODO SOUND
};

