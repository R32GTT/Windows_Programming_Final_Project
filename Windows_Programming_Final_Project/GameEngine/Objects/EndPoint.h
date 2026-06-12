#pragma once
#include "GameObject.h"
#include "Enums.h"

class EndPoint : public GameObject
{
public:
	EndPoint();
	virtual ~EndPoint();
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha);

	virtual void OnCollision(GameObject* other) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;

	std::string GetMapName() { return _targetMapName; }
	void SetMapName(const std::string& str) { _targetMapName = str; }


private:
	std::string _targetMapName = "";
};
