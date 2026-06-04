#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;

};