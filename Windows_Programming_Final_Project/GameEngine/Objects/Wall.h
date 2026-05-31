#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Wall : public GameObject {
private:
	D2D1_RECT_F wallCoords{};
	bool is_Transparent{ FALSE };

public:


	virtual ~Wall();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc, float alpha) override;

	Wall() {
		type = OBJECTTYPE::WALL;
		layer = Layers::WALL;
	}

	Wall(POINT start, POINT end);


};