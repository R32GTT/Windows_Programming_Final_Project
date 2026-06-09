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
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;
	virtual bool IsKilled() override;
	virtual void OnCollision(GameObject* other) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;
	

	void SetWeaponType(WPTYPE type);


	WPTYPE GetWeaponType() const;

	
	PlayerState Move();

	



	
private:

	//무기 유무 변수 추가
	bool is_Item = false;

	//무기 버림유무 변수 추가
	bool is_drop = false;

	bool _isAttacking = false; // 공격 중인지 체크하는 플래그

	//어떤 무기를 가지고 있는지 변수(플레이어 용)
	WPTYPE currentWeapon_Player = WPTYPE::NONE;

	PlayerState status = PlayerState::IDLE;
	
	FlipBook* _anims[static_cast<int>(PlayerState::DEAD) + 1]{};
};
