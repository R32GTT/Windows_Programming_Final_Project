#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"
enum class DecoType
{
	Fridge = 0,
	Burner,
	Deco_Count,
};

class DECO : public GameObject {

private:
	D2D1_RECT_F decoCoords{};

	FlipBook* _Danims[(int)DecoType::Deco_Count] = { nullptr };
	DecoType _decoType;

public:

	

	virtual ~DECO();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	virtual void SaveToData(ObjectSpawnData& outData) override
	{
		GameObject::SaveToData(outData);
	}

	virtual void LoadFromData(const ObjectSpawnData& spawnData) override
	{

		GameObject::LoadFromData(spawnData);
	}

	void ChangeDecoType(DecoType type);

	DECO() {
		type = OBJECTTYPE::DECO;
		layer = Layers::DECO;
	}

	DECO(POINT start, POINT end) {
		type = OBJECTTYPE::DECO;
		layer = Layers::DECO;
		decoCoords.left = start.x; decoCoords.top = start.y;
		decoCoords.right = end.x; decoCoords.bottom = end.y;
	}
};