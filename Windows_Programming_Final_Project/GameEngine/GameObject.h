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
	int layer{};
	OBJECTTYPE type = OBJECTTYPE::NONE;
};

class Player : public GameObject {

private:
	//죽음 유무 Player로 이동함
	bool is_Dead = false;

public:

	//플레이어 이동 공격 아이템 획득 함수
	//Move()함수의 매개변수와 
	//void Move();
	//void Attack();
	//void GetItem();

	//둘이 달라야 하나??? 죽음
	//죽음 체크하는 함수(플레이어 버전 추가)
	bool CheckDead_P() {
		return is_Dead = true;
	}

};

class Enemy : public GameObject {

private:
	//기절과 죽음 유무 Enemy로 이동함
	bool is_Unconsious = false;
	bool is_Dead = false;

public:

	//적 이동 적 공격 적 시야 함수
	//void EmMove();
	//void EmAttack();
	//void EmSight();

	//죽음 체크하는 함수(적 버전 추가)
	bool CheckDead_E() {
		return is_Dead = true;
	}

};

//I need to change this....
//bool is_Hit = false;
//Change ITEM class to Weapon Class

class Weapon : public GameObject {

private:
	//무기가 총인지 빠루인지 구분
	//처음에는 주먹만 있어서 true로 설정

	//그러면 enum으로 관리할까???
	//enum방식은 후에 의논해봅세
	bool is_Gun = false;
	bool is_MEELE = false;
	bool is_Fist = true;

	//근거리 무기 리치 변수
	int leach_MEELE = 0;
	//근거리 무기 공격속도 변수
	int Speed_MEELE = 0;

	//원거리 무기용 변수 추가
	//탄약용 변수
	int ammo = 0;

	//총 공격속도 변수
	int Speed_Gun = 0; 


public:

	//무기 체크 함수
	//주먹용 체크
	//근거리 체크
	//원거리 체크

	//주먹 체크함수
	bool GetFist() {
		return is_Fist = true;
	}

	//근접무기 체크함수
	bool GetMEE() {
		return is_MEELE = true;
	}

	//원거리무기 체크함수
	bool GetGun() {
		return is_Gun = true;
	}




};