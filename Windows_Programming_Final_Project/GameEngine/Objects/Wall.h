#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Wall : public GameObject {
private:
	D2D1_RECT_F wallCoords{};
	bool is_Transparent{ FALSE };

public:

	Wall();
	virtual ~Wall();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	Wall() {
		type = OBJECTTYPE::WALL;
	}

	Wall(POINT start, POINT end)
	{
		type = OBJECTTYPE::WALL;
		wallCoords.left = start.x; wallCoords.top = start.y;
		wallCoords.right = end.x; wallCoords.bottom = end.y;
	}


};