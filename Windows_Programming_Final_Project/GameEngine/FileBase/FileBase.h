#pragma once
class FileBase
{
	FileBase();
	virtual ~FileBase();

	virtual void LoadFile(const std::wstring& path);
	virtual void SaveFile(const std::wstring& path);

};

