#include "pch.h"
#include "Deco.h"
#include "../LevelData/LevelData.h"

DECO::~DECO()
{
}

void DECO::Init()
{

}

void DECO::Update()
{


}

void DECO::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (renderTarget == nullptr) return;

	Vec2<float> renderPos = GetRenderPos(alpha);

	D2D1_RECT_F rect = D2D1::RectF(renderPos.x - _halfSize.x, renderPos.y - _halfSize.y, renderPos.x + _halfSize.x, renderPos.y + _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGreen), &brush);

	if (brush != nullptr)
	{
		renderTarget->FillRectangle(rect, brush);
		brush->Release();
	}


}

void DECO::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
}

void DECO::LoadFromData(const ObjectSpawnData& spawnData)
{

	GameObject::LoadFromData(spawnData);
}
