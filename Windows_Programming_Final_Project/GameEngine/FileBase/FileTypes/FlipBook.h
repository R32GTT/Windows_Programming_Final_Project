#pragma once
#include "pch.h"
#include "../FileBase.h"

class Texture;

struct FlipbookInfo
{
	Texture* texture{};
	std::wstring name{};
	Vec2<int> size{};
	int start{};
	int end{};
	int line{};
	float duration{ 1.f };
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
