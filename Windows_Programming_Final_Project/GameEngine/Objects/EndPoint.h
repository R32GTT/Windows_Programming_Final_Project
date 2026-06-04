#pragma once

class EndPoint : public GameObject
{


	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;
};
