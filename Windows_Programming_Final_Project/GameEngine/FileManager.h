#pragma once


class FileManager
{
public:

	DECLARE_SINGLE(FileManager);

	~FileManager();

public:
	void Init(HWND, fs::path filePath);
	void Clear();

	const fs::path& GetFilePath() { return filePath; };


private:
	HWND hWnd{};
	fs::path filePath{};
	

};

