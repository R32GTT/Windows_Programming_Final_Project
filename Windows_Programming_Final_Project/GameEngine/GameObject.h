#pragma once
#include "pch.h"
#include "../Enums.h"


class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC);
	
	virtual bool CheckDead();

	Layers GetLayer() { return layer; };


	Vec2<float> pos{};
	Vec2<float> facingDir{};
	Vec2<float> movingDir{};
	float speed{};
	
	Layers layer = Layers::ACTORS;
	
	OBJECTTYPE type = OBJECTTYPE::NONE;

	//플레이어 상태를 위한 변수 추가
	PlayerState status = PlayerState::IDLE;
};

//class Player 수정하기
//기존의 죽음 유무 그리고 처형 변수 제거
class Player : public GameObject {

private:
	
	//무기 유무 변수 추가
	bool is_Item = false;

	//무기 버림유무 변수 추가
	bool is_drop = false;

public:

	//플레이어 이동 함수
	//이동 함수 보류
	void Move() {

		status = PlayerState::MOVE;

		//pos.x 
		//pos.y

		//facingDir.x
		//facingDir.y

		//movingDir.x
		//movingDir.y

	}
	//void Attack();
	//void GetItem();

	//둘이 달라야 하나??? 죽음
	//죽음 체크하는 함수(플레이어 버전 추가)
	//죽음 체크하는 함수 virtual 선언 해서 바꿈. 이제 플레이어나 적이나 둘 다 동일한 함수로 체크함.
	/*void CheckDead() override { //살아있으면 FALSE, 죽었으면 TRUE
		status = PlayerState::DEAD;
		//return status;
	}*/

	//플레이어 무기유무 함수 수정하기
	//HasWeapon에서 주먹 근거리 원거리 체크하고
	bool HasWeapon() {
		
		//Weapon::What_Weapon
		//무기는 Weapon을 쓸까 아니면 ITEM을 사용할까???

	}

	//무기를 버렸는지 유무확인
	bool DropWeapon() {

		if (is_drop == true) {
			return (is_drop);
		}
		else {
			is_drop = false;
			return (is_drop);
		}

	}

	//플레이어가 가만히 있을때
	bool is_still() {
		status = PlayerState::IDLE;
	}

	//플레이어가 죽었는지 확인할때
	bool is_alive() {

		//if(false)
		status = PlayerState::DEAD;
		//else
		//alive true

	}


	//플레이어 무기 획득 함수 추가
	bool GetItem() {
		return (is_Item);
	}

	//플레이어 적 처형 함수 추가
	bool Execute() {
		//return (is_ExeCute);
		status = PlayerState::EXECUTE;
	}

};

class Enemy : public GameObject {

private:
	//기절과 죽음 유무 Enemy로 이동함
	bool is_Unconsious = false;
	bool is_Dead = false;

public:

	//적 이동 함수
	//보류
	void EmMove() {

		//pos.x 
		//pos.y

		//facingDir.x
		//facingDir.y

		//movingDir.x
		//movingDir.y


	}

	//적 공격 적 시야 적 이동범위 함수 뼈대
	//void EmAttack();
	//void EmSight();
	//void EnemyRange();

	//죽음 체크하는 함수(적 버전 추가)
	//플레이어랑 적이랑 다를 필요가 없어서 VIRTUAL 선언함
	bool CheckDead() override { //살아있으면 FALSE, 죽었으면 TRUE
		return (is_Dead);
	}

	//적 기절을 체크하는 함수
	//플레이어와 다르게 적은 기절이 있다
	//기절이면 TRUE 사망이면 FALSE
	bool CheckFaint() {
		if (is_Unconsious == true) {
			return (is_Unconsious);
		}
		else if (is_Unconsious != true) {
			return (is_Dead);
		}
	}

};

class Wall : public GameObject {
private:
	D2D1_RECT_F wallCoords{};
	bool is_Transparent{ FALSE };

public:
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

//바닥 클래스 수정
//바닥 클래스 left top right bottom 추가
class Floor : public GameObject {
private:
	D2D1_RECT_F floorCoords{};

public:
	Floor(){
		layer = Layers::FLOOR;
	}

	Floor(POINT start, POINT end)
	{
		layer = Layers::FLOOR;
		floorCoords.left = start.x; floorCoords.top = start.y;
		floorCoords.right = end.x; floorCoords.bottom = end.y;
	}
};


class DECO : public GameObject {

private:
	D2D1_RECT_F decoCoords{};
public:
	DECO() {
		type = OBJECTTYPE::DECO;
	}

	DECO(POINT start, POINT end) {
		type = OBJECTTYPE::DECO;
		decoCoords.left = start.x; decoCoords.top = start.y;
		decoCoords.right = end.x; decoCoords.bottom = end.y;
	}
};

//I need to change this....
//bool is_Hit = false;
//Change ITEM class to Weapon Class
class Weapon : public GameObject {

private:
	
	enum class WPTYPE {
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

	bool is_RIFLE = false;
	bool is_Meele = false;
	bool is_Fist = true;

	//총이 지금 발사되고 있는지 판단 변수
	bool is_Shoot = false;

	//탄창 다 사용했는지 아닌지 유무판단
	bool is_OUT = false;

	// 공용 변수들? 굳이 나눌 필요 없을거 같았음
	unsigned __int64 attack_Speed = 0;
	unsigned __int64 projectile_Life = 0; // 투사체 수명 정하는 것. 근접공격은 공격 프레임 동안만 스폰하게, 총알은 어딘가에 충돌할때까지.
	float projecttile_Speed = 0.0f;
	int projectile_Count = 1; // 나중에 샷건 만든다면 이거 쓸거.
	float projectile_Deviation = 0.0f; // 근접 쓸땐 0이지만, 원거리 무기는 탄퍼짐이 있어야 하니... 라디안 값임(ATAN 쓰니)


	//근거리 무기 리치 변수
	int reach_Meele = 0;	

	//원거리 무기용 변수 추가
	//탄약용 변수
	int ammo = 0;




public:

	//무기 체크 함수
	//수정해서 하나로 합치기 

	//어떤 무기인지 체크하는 함수(주먹인지 근접무기인지 원거리 무기인지)
	bool What_Weapon() {

		if (is_Fist) {
			return (is_Fist);
		}
		else if (is_Meele) {
			return (is_Meele);
		}
		else {
			return (is_RIFLE);
		}

	}

	//총이 발사되는 중임을 보는 함수
	//탄창수에 따라 살짝 변경함
	bool Shooting() {
		if (ammo > 0) {
			is_Shoot = true;
			ammo--;
			return (is_Shoot);
		}
	}

	//탄창 빈 유무 체크함수
	//다시 살짝 변경 함
	//ammo가 0이 될때 is_Shoot을 false로 하고
	//비었음을 리턴한다
	bool CheckAmo() {
		if (ammo == 0) {
			is_Shoot = false;
			return (is_OUT);
		}
	}


};