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
	
	virtual bool CheckDead();

	Vec2<float> pos{};
	Vec2<float> dir{};
	float speed{};
	int layer{};
	// sprite 타입도 넣어놔야 할듯.
	// DX2D 쓰려면 HBITMAP 넣어두는건 제쳐두고...
	OBJECTTYPE type = OBJECTTYPE::NONE;
};

class Player : public GameObject {

private:
	//죽음 유무 Player로 이동함
	bool is_Dead = false;

	//무기 유무 변수 추가
	bool is_Item = false;

	//적 처형 변수 추가
	bool is_ExeCute = false;

public:

	//플레이어 이동 공격 아이템 획득 함수
	//Move()함수의 매개변수와 
	//void Move();
	//void Attack();
	//void GetItem();

	//둘이 달라야 하나??? 죽음
	//죽음 체크하는 함수(플레이어 버전 추가)
	//죽음 체크하는 함수 virtual 선언 해서 바꿈. 이제 플레이어나 적이나 둘 다 동일한 함수로 체크함.
	bool CheckDead() override { //살아있으면 FALSE, 죽었으면 TRUE
		return (is_Dead);
	}

	//플레이어 무기 획득 함수 추가
	bool GetItem() {
		return (is_Item);
	}

	//플레이어 적 처형 함수 추가
	bool Execute() {
		return (is_ExeCute);
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
	//플레이어랑 적이랑 다를 필요가 없어서 VIRTUAL 선언함
	bool CheckDead() override { //살아있으면 FALSE, 죽었으면 TRUE
		return (is_Dead);
	}

};

class Wall : public GameObject {
private:
	D2D1_RECT_F wallCoords{};
	bool is_Transparent{ FALSE };

public:
	Wall() {
		layer = 5;
		type = OBJECTTYPE::WALL;
	}
	
	Wall(POINT start, POINT end)
	{
		layer = 5;
		type = OBJECTTYPE::WALL;
		wallCoords.left = start.x; wallCoords.top = start.y;
		wallCoords.right = end.x; wallCoords.bottom = end.y;
	}

	
};

class Floor : public GameObject {
private:
	D2D1_RECT_F floorCoords{};

public:
	Floor(){
		layer = 1;
		type = OBJECTTYPE::FLOOR;
	}

	Floor(POINT start, POINT end)
	{

	}
};

//벽이 아닌 객체용 클래스
class ENTITY : public GameObject {

private:

public:
	ENTITY() {

	}

	ENTITY() {

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
	//제안서 내용 토대로 샷건에서 소총으로 교체
	//주석 SHOTGUN 처리 -> RIFLE(소총)으로 교체

	enum WPTYPE {
		NONE =0,
		FIST,
		//KNIFE, TODO Maybe someday...
		CROWBAR,
		//AXE,
		//PISTOL,
		//SHOTGUN,
		RIFLE,
		TOTAL_COUNT
	};

	//Gun -> RIFLE로 수정함
	bool is_RIFLE = false;
	bool is_Meele = false;
	bool is_Fist = true;

	// 공용 변수들? 굳이 나눌 필요 없을거 같았음
	unsigned __int64 attack_Speed = 0;
	unsigned __int64 projectile_Life = 0; // 투사체 수명 정하는 것. 근접공격은 공격 프레임 동안만 스폰하게, 총알은 어딘가에 충돌할때까지.
	float projecttile_Speed = 0.0f;


	//근거리 무기 리치 변수
	int reach_Meele = 0;	

	//원거리 무기용 변수 추가
	//탄약용 변수
	int ammo = 0;

	


public:

	//무기 체크 함수
	//주먹용 체크
	//근거리 체크
	//원거리 체크

	//주먹 체크함수
	bool GetFist() {
		return (is_Fist);
	}

	//근접무기 체크함수
	bool GetMeele() {
		return (is_Meele);
	}

	//원거리무기 체크함수
	bool GetGun() {
		return (is_RIFLE);
	}




};