 #pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Player : public GameObject {
public:

	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc, float alpha) override;
	

	//플레이어 무기유무 함수 수정하기
	//HasWeapon에서 주먹 근거리 원거리 체크하고
	bool HasWeapon() {

		//Weapon::What_Weapon
		//무기는 Weapon을 쓸까 아니면 ITEM을 사용할까???

	}
	//플레이어 이동 함수
	//이동 함수 보류
	//이동 함수 PlayerState::MOVE 추가
	PlayerState Move();

	//둘이 달라야 하나??? 죽음
	//죽음 체크하는 함수(플레이어 버전 추가)
	//죽음 체크하는 함수 virtual 선언 해서 바꿈. 이제 플레이어나 적이나 둘 다 동일한 함수로 체크함.
	/*void CheckDead() override { //살아있으면 FALSE, 죽었으면 TRUE
		status = PlayerState::DEAD;
		//return status;
	}*/

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
	//PlayerState
	//bool말고 enum을 이용하여 함수를 수정함
	PlayerState is_still() {
		status = PlayerState::IDLE;
		return (status);
	}

	//플레이어가 죽었는지 확인할때
	//살아있을때는 어떻게 구현하지???
	PlayerState is_alive() {

		if (status != PlayerState::DEAD) {
			return (status);
		}
		else if (status == PlayerState::DEAD) {
			return (status);
		}

	}

	//플레이어 무기 획득 함수 추가
	bool GetItem() {
		return (is_Item);
	}

	//플레이어 적 처형 함수 추가
	//플레이어 적 처형 함수 변형
	PlayerState Execute() {
		status = PlayerState::EXECUTE;
		return (status);
	}

private:

	//무기 유무 변수 추가
	bool is_Item = false;

	//무기 버림유무 변수 추가
	bool is_drop = false;

	PlayerState status = PlayerState::IDLE;

};
