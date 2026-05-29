#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class DECO : public GameObject {

private:
	D2D1_RECT_F decoCoords{};
public:

	virtual ~DECO();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc, float alpha) override;

	DECO() {
		type = OBJECTTYPE::DECO;
	}

	DECO(POINT start, POINT end) {
		type = OBJECTTYPE::DECO;
		decoCoords.left = start.x; decoCoords.top = start.y;
		decoCoords.right = end.x; decoCoords.bottom = end.y;
	}
};