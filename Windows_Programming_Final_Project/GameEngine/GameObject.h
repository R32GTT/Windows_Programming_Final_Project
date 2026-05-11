#pragma once
#include "Utility/Vec2.h"

class GameObject
{
public:

	enum {
		NONE = 0,
		PLAYER,
		ENEMY,
		WEAPON,
	};

	
	Vec2<float> direction{};
	float speed{};
	float x{}, y{};
	

};

