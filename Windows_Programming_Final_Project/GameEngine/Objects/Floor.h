#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Floor : public GameObject {
private:
	D2D1_RECT_F floorCoords{};

public:


	virtual ~Floor();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;


	Floor() {
		layer = Layers::FLOOR;
	}

	Floor(POINT start, POINT end)
	{
		layer = Layers::FLOOR;
		floorCoords.left = start.x; floorCoords.top = start.y;
		floorCoords.right = end.x; floorCoords.bottom = end.y;
	}
};