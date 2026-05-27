#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Weapon : public GameObject {

private:

	enum class WPTYPE {
		NONE = 0,
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
	//변수 변경 0 -> 26
	int ammo = 26;

public:

	Weapon();
	virtual ~Weapon();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	//무기 체크 함수
	//수정해서 하나로 합치기 

	//어떤 무기인지 체크하는 함수(주먹인지 근접무기인지 원거리 무기인지)
	//OBJECTTYPE를 이용하여 변경
	bool What_Weapon() {

		type = OBJECTTYPE::WEAPON;

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