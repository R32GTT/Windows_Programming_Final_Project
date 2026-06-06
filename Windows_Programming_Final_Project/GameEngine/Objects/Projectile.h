#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile();
	virtual ~Projectile();

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;

};