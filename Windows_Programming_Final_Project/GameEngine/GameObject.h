#pragma once
#include "Utility/Vec2.h"

class GameObject
{
public:

	enum class OBJECTTYPE{
		NONE = 0,
		PLAYER = 1,
		ENEMY = 2,
		WEAPON = 3,
		WALL
	};



	
	Vec2<float> posDir{};
	float speed{};
	bool is_Unconsious = false;
	bool is_Dead = false;
	OBJECTTYPE type = OBJECTTYPE::NONE;

};

