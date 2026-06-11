#include "pch.h"
#include "Deco.h"
#include "../LevelData/LevelData.h"
#include "GameObject.h"
#include "Managers.h"

DECO::~DECO()
{
}

void DECO::Init()
{
	FileManager* FM = GET_SINGLE(FileManager);

	_Danims[(int)DecoType::Fridge] = FM->GetFlipBook(L"Deco1CharAnim_Idle");
	_Danims[(int)DecoType::Burner] = FM->GetFlipBook(L"Deco2CharAnim_Idle");

	PlayAnimation(_Danims[(int)_decoType]);
}

void DECO::Update()
{


}

void DECO::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	/*if (renderTarget == nullptr) return;

	Vec2<float> renderPos = GetRenderPos(alpha);

	D2D1_RECT_F rect = D2D1::RectF(renderPos.x - _halfSize.x, renderPos.y - _halfSize.y, renderPos.x + _halfSize.x, renderPos.y + _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGreen), &brush);

	if (brush != nullptr)
	{
		renderTarget->FillRectangle(rect, brush);
		brush->Release();
	}*/

	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);
}

void DECO::ChangeDecoType(DecoType type)
{
	_decoType = type;
	PlayAnimation(_Danims[(int)_decoType]);
}

