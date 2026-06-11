#include "pch.h"
#include "Weapon.h"
#include "../LevelData/LevelData.h"
#include "Managers.h"


constexpr float shortMelee{ 50.0f };
constexpr float midMelee{ 100.0f };
constexpr float longMelee{ 150.0f };

//struct WeaponInfo
//{
//	unsigned __int64			attack_Speed = 0;
//	unsigned __int64			projectile_Life = 0; // 투사체 수명 정하는 것. 근접공격은 공격 프레임 동안만 스폰하게, 총알은 어딘가에 충돌할때까지.
//	float						projectile_Speed = 0.0f;
//	int							projectile_Count = 1; // 나중에 샷건 만든다면 이거 쓸거.
//	float						projectile_Deviation = 0.0f; // 근접 쓸땐 0이지만, 원거리 무기는 탄퍼짐이 있어야 하니... 라디안 값임(ATAN 쓰니)
//	Projectile::ProjectileType	projType = Projectile::ProjectileType::MELEE;
//	bool						isLethal = true;
//
//	int							maxAmmo{};
//	Vec2F						_projSize{};
//};

static const WeaponInfo g_WeaponTable[(int)WPTYPE::TOTAL_COUNT] =
{
	// [0] WPTYPE::NONE
	{ 0, 0, 0.0f, 0, 0.0f, Projectile::ProjectileType::MELEE, false, 0, Vec2F(0.0f, 0.0f) },

	// [1] WPTYPE::FIST
	{
		200,
		50,
		0.0f,
		1,
		0.0f,
		Projectile::ProjectileType::MELEE,
		false,
		-1,//inf ammo
		Vec2F(shortMelee * 0.5f, 25.0f)
	},

	// [2] WPTYPE::CROWBAR (빠루)
	{
		300,                            // attack_Speed (0.3초 쿨타임)
		80,                             // projectile_Life (0.08초 유지)
		0.0f,
		1,
		0.0f,
		Projectile::ProjectileType::MELEE,
		true,
		-1,
		Vec2F(midMelee * 0.5f, 35.0f)
	},

	// [3] WPTYPE::RIFLE (라이플)
	{
		50,
		20000,                           // projectile_Life (최대 2초 동안 총알 날아감)
		1500.0f,                        // projectile_Speed
		1,
		0.06f,                          // projectile_Deviation (라디안 탄퍼짐)
		Projectile::ProjectileType::BULLET,
		true,
		26,
		Vec2F(2.5f, 2.5f)               // projectile_Size (총알 투사체 크기)
	}
};

const WeaponInfo& GetWeaponInfo(WPTYPE type)
{
	return g_WeaponTable[static_cast<int>(type)];
}

Weapon::Weapon()
{
	type = OBJECTTYPE::WEAPON;
	layer = Layers::ITEM;
}

Weapon::~Weapon()
{
}

void Weapon::Init()
{
	FileManager* FM = GET_SINGLE(FileManager);
	_anims[(int)AnimType::IDLE_CROWBAR] = FM->GetFlipBook(L"CrowCharAnim_Idle");
	_anims[(int)AnimType::IDLE_GUN] = FM->GetFlipBook(L"GunCharAnim_Idle");
	if(_weaponType == WPTYPE::CROWBAR)
		PlayAnimation(_anims[(int)AnimType::IDLE_CROWBAR]);
	if (_weaponType == WPTYPE::RIFLE)
		PlayAnimation(_anims[(int)AnimType::IDLE_GUN]);
}

void Weapon::Update()
{
	/*if (owner != nullptr)
	{
		
		this->pos = owner->GetPos();
	}*/
}

void Weapon::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);

}

void Weapon::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.weaponType = _weaponType;
}

void Weapon::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	_weaponType = spawnData.weaponType;
}

void Weapon::SetWeaponType(WPTYPE type)
{
	_weaponType = type;
	switch (_weaponType)
	{
	case WPTYPE::CROWBAR:
		_currentAmmo = GetWeaponInfo(type).maxAmmo;
		PlayAnimation(_anims[(int)AnimType::IDLE_CROWBAR]);
		break;
	case WPTYPE::RIFLE:
		_currentAmmo = GetWeaponInfo(type).maxAmmo;
		PlayAnimation(_anims[(int)AnimType::IDLE_GUN]);
		break;
	default:
		break;
	}
}
