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
}

void Weapon::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
}

void Weapon::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
}

void Weapon::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
}
