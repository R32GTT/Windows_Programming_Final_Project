#pragma once
#include "Utility/Vec2.h"


class GameObject
{
public:

	GameObject();
	~GameObject();


	

	enum TYPE {
		NONE = 0,
		PLAYER,
		ENEMY,
		WEAPON,
		PROJECTILE,
		WALL
	};
	
	Vec2<float> pos{};
	Vec2<float> dir{};
	float speed{};
	bool is_Unconsious = false;
	bool is_Dead = false;
	TYPE type = TYPE::NONE;

};

