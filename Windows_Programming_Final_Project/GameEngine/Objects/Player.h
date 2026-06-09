 #pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

//무기 클래스 전방 선언
class Weapon;

class Player : public GameObject {
public:

	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;
	

	void SetWeaponType(WPTYPE type);
	WPTYPE GetWeaponType() const;

	
	PlayerState Move();

	//무기 장착 및 해제를 위한 함수
	void EquipWeapon(Weapon* weapon);
	void DropWeapon();
	Weapon* GetCurrentWeapon() const { return currentWeapon; }

	
private:

	//무기 유무 변수 추가
	bool is_Item = false;

	//무기 버림유무 변수 추가
	bool is_drop = false;

	bool _isAttacking = false; // 공격 중인지 체크하는 플래그

	//어떤 무기를 가지고 있는지 변수(플레이어 용)
	WPTYPE currentWeapon_Player = WPTYPE::NONE;

	//실제 장착 중인 무기 객체를 가리키는 포인터 추가
	Weapon* currentWeapon = nullptr;

	PlayerState status = PlayerState::IDLE;
	
	FlipBook* _anims[static_cast<int>(PlayerState::DEAD) + 1]{};
};
