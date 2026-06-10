#include "pch.h"
#include "Weapon.h"
#include "../LevelData/LevelData.h"

Weapon::Weapon()
{
	type = OBJECTTYPE::WEAPON;
	layer = Layers::ITEM;
}

Weapon::~Weapon()
{
}

void Weapon::Init()
{
}

void Weapon::Update()
{
	if (owner != nullptr)
	{
		this->pos = owner->GetPos();
	}

}

void Weapon::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2<float> renderPos = GetRenderPos(alpha);

	//임시로 가로 20 세로 10 크기의 사각형(총 모양처럼) 그려본다
	D2D1_RECT_F rect = D2D1::RectF(renderPos.x - 10.0f, renderPos.y - 5.0f, renderPos.x + 10.0f, renderPos.y + 5.0f);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &brush);

	if (brush != nullptr)
	{
		renderTarget->FillRectangle(rect, brush);
		brush->Release();
	}



}

void Weapon::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
}

void Weapon::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
}
