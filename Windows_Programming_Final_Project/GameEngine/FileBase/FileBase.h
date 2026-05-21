#pragma once
class FileBase
{
public:
	FileBase();
	virtual ~FileBase();

	virtual void LoadFile(const std::wstring& path);
	virtual void SaveFile(const std::wstring& path);

};

