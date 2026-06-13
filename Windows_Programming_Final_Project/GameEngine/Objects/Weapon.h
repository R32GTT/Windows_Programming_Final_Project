#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

struct WeaponInfo
{
	// 공용 변수들? 굳이 나눌 필요 없을거 같았음
	unsigned __int64			attack_Speed = 0;
	float			projectile_Life = 0; // 투사체 수명 정하는 것. 근접공격은 공격 프레임 동안만 스폰하게, 총알은 어딘가에 충돌할때까지.
	float						projectile_Speed = 0.0f;
	int							projectile_Count = 1; // 나중에 샷건 만든다면 이거 쓸거.
	float						projectile_Deviation = 0.0f; // 근접 쓸땐 0이지만, 원거리 무기는 탄퍼짐이 있어야 하니... 라디안 값임(ATAN 쓰니)
	ProjectileType				__projType = ProjectileType::MELEE;
	bool						isLethal = true;
	
	int							maxAmmo{};
	Vec2F						_projSize{};
};

const WeaponInfo& GetWeaponInfo(WPTYPE wpType);

class Weapon : public GameObject {

private:

	
	WPTYPE _weaponType = WPTYPE::CROWBAR; //빠루 아님 총이라서 이대로 냅둠
	int _currentAmmo{};

public:

	Weapon();
	virtual ~Weapon();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;

	void SetWeaponType(WPTYPE type);
	WPTYPE GetWeaponType() const { return _weaponType; };
	
	void SetAmmo(int ammo) { _currentAmmo = ammo; };
	int GetAmmo() const { return _currentAmmo; };


};