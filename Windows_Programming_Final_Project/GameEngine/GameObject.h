#pragma once
#include "Utility/Vec2.h"
#include "../pch.h"

class GameObject
{
public:

	enum OBJECTTYPE{
		NONE = 0,
		PLAYER = 1,
		ENEMY = 2,
		WEAPON = 3,
		WALL
	};

	void Init();
	void Update();
	void Render(HDC);


	Vec2<float> pos{};
	Vec2<float> dir{};
	float speed{};
	bool is_Unconsious = false;
	bool is_Dead = false;
	OBJECTTYPE type = OBJECTTYPE::NONE;

};

class Player : GameObject {

private:


public:

};

class Enemy : GameObject {

private:

public:


};

class ITEM : GameObject {

private:

public:

};