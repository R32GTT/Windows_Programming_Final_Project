#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Enemy : public GameObject {

private:
	//기절과 죽음 유무 Enemy로 이동함
	bool is_Unconsious = false;
	bool is_Dead = false;
	EnemyType _enemyType = EnemyType::NORMAL;

public:

	Enemy();
	virtual ~Enemy();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;


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

	//일반적인지 덩치인지 구분하는 함수
	EnemyType What_Enemy() {

		type = OBJECTTYPE::ENEMY;

		if (_enemyType != EnemyType::NORMAL) {
			_enemyType = EnemyType::ARMORED;
			return (_enemyType);
		}
		else {
			return (_enemyType);
		}
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