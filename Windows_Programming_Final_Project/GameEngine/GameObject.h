#pragma once
#include "pch.h"
#include "Utility/Vec2.h"


class GameObject
{
public:

	enum OBJECTTYPE{
		NONE = 0,
		PLAYER = 1,
		ENEMY = 2,
		WEAPON = 3,
		PROJECTILE,
		DECO,
		WALL,
		FLOOR
	};

	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC);


	Vec2<float> pos{};
	Vec2<float> dir{};
	float speed{};
	bool is_Unconsious = false;
	bool is_Dead = false;
	int layer{};
	OBJECTTYPE type = OBJECTTYPE::NONE;
};

class Player : public GameObject {

private:

public:

	//플레이어 이동 공격 아이템 획득 함수
	void Move();
	//void Attack();
	//void GetItem();
	

};

class Enemy : public GameObject {

private:

public:

	//적 이동 적 공격 적 시야 함수
	//void EmMove();
	//void EmAttack();
	//void EmSight();


};

class ITEM : public GameObject {

private:
	//무기가 총인지 빠루인지 구분
	bool is_Gun = false;
	bool is_Stick = false;
	bool Basic_Fist = true;

public:



};