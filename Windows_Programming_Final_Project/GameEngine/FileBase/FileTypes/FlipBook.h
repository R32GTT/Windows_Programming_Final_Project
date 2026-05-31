#pragma once
#include "pch.h"
#include "../FileBase.h"

class Texture;
class Sprite;

struct FlipbookInfo
{
	std::wstring name{};
	std::vector<Sprite*> frames{}; 
	float duration{ 0.1f };        
	bool loop{ true };
};

class FlipBook : public FileBase
{
public:
	FlipBook();
	virtual ~FlipBook();

	void SetInfo(const FlipbookInfo& info) { _info = info; };
	const FlipbookInfo& GetInfo() { return _info; };

private:
	FlipbookInfo _info;
};
