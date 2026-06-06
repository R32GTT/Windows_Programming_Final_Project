#include "pch.h"
#include "Projectile.h"

Projectile::Projectile()
{
	type = OBJECTTYPE::PROJECTILE;
	layer = Layers::ACTORS;
}

Projectile::~Projectile()
{
}

void Projectile::SaveToData(ObjectSpawnData& outData)
{
}

void Projectile::LoadFromData(const ObjectSpawnData& spawnData)
{
}
