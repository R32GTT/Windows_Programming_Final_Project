#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

struct WeaponInfo
{
	// 공용 변수들? 굳이 나눌 필요 없을거 같았음
	unsigned __int64 attack_Speed = 0;
	unsigned __int64 projectile_Life = 0; // 투사체 수명 정하는 것. 근접공격은 공격 프레임 동안만 스폰하게, 총알은 어딘가에 충돌할때까지.
	float projecttile_Speed = 0.0f;
	int projectile_Count = 1; // 나중에 샷건 만든다면 이거 쓸거.
	float projectile_Deviation = 0.0f; // 근접 쓸땐 0이지만, 원거리 무기는 탄퍼짐이 있어야 하니... 라디안 값임(ATAN 쓰니)
	const float shortMelee{ 50.0f };
	const float midMelee{ 100.0f };
	const float longMelee{ 150.0f };
	int ammo = 26;
};


class Weapon : public GameObject {

private:

	//무기 소유자(플레이어 또는 적)
	//무기가 실제로 장착이 되게끔 구현 시작
	//GameObject* owner = nullptr;

	WPTYPE _weaponType = WPTYPE::CROWBAR;

	WeaponInfo _wpInfo{};


	//근거리 무기 리치 변수
	

	//원거리 무기용 변수 추가
	//탄약용 변수
	//변수 변경 0 -> 26


public:

	Weapon();
	virtual ~Weapon();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;

	////무기 장착 및 해제를 위한 함수 추가
	//void SetOwner(GameObject* newOwner) { owner = newOwner; }
	//GameObject* GetOwner() const { return owner; }
	//
	//



	//탄창 빈 유무 체크함수
	//다시 살짝 변경 함
	//ammo가 0이 될때 is_Shoot을 false로 하고
	//비었음을 리턴한다
	bool CheckAmmo() {
		return _wpInfo.ammo > 0 ? true : false; 
	};

};