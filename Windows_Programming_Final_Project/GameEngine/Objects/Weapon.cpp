#include "pch.h"
#include "Weapon.h"
#include "../LevelData/LevelData.h"
#include "Managers.h"

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
	FileManager* FM = GET_SINGLE(FileManager);
	_anims[(int)AnimType::IDLE] = FM->GetFlipBook(L"GunCharAnim_Idle");
	PlayAnimation(_anims[(int)AnimType::IDLE]);
}

void Weapon::Update()
{
	/*if (owner != nullptr)
	{
		
		this->pos = owner->GetPos();
	}*/

	PlayAnimation(_anims[(int)AnimType::IDLE]);
}

void Weapon::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);

}

void Weapon::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.weaponType = _weaponType;
}

void Weapon::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	_weaponType = spawnData.weaponType;
}
