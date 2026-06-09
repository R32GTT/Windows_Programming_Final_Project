#pragma once
#include "GameObject.h"
#include "Enums.h"

class EndPoint : public GameObject
{
public:
	EndPoint();
	virtual ~EndPoint();

	virtual void OnCollision(GameObject* other) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;
};
