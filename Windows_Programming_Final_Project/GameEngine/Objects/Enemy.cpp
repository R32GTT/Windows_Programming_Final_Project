#include "pch.h"
#include "Enemy.h"
#include "Managers.h"
#include "Objects.h"
#include "../LevelData/LevelData.h"
#include "../FileBase/FileTypes/FlipBook.h"

Enemy::Enemy()
{
	speed = baseEnemySpeed;
	_halfSize = Vec2F(25.0f, 10.0f);

	type = OBJECTTYPE::ENEMY;
	layer = Layers::ACTORS;
}

Enemy::Enemy(EnemyType etype)
{
	_halfSize = Vec2F(25.0f, 10.0f);
	type = OBJECTTYPE::ENEMY;
	layer = Layers::ACTORS;

	if (etype == EnemyType::NORMAL)
		speed = baseEnemySpeed;
	else
		speed = armoredEnemySpeed;
	_enemyType = etype;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	FileManager* FM = GET_SINGLE(FileManager);

	{
		_anims[(int)AnimType::IDLE] = FM->GetFlipBook(L"MobCharAnim_Idle");
		_anims[(int)AnimType::MOVE] = FM->GetFlipBook(L"MobCharAnim_Walking");
		_anims[(int)AnimType::ATTACK_FIST] = FM->GetFlipBook(L"MobCharAnim_Punch");
		_anims[(int)AnimType::ATTACK_CROWBAR] = FM->GetFlipBook(L"MobCharAnim_Crowbar");
		_anims[(int)AnimType::ATTACK_GUN] = FM->GetFlipBook(L"MobCharAnim_Rifle Shooting");
		_anims[(int)AnimType::EXECUTE] = FM->GetFlipBook(L"MobCharAnim_Execution");
		_anims[(int)AnimType::DEAD] = FM->GetFlipBook(L"MobCharAnim_Dead");
		_anims[(int)AnimType::UNCONSCIOUS] = FM->GetFlipBook(L"MobCharAnim_Unconscious");

		_anims[(int)AnimType::IDLE_CROWBAR] = FM->CreateFlipBook(L"MobCharAnim_Crowbar_Idle_Auto");
		if (_anims[(int)AnimType::IDLE_CROWBAR]->GetInfo().frames.empty() && _anims[(int)AnimType::ATTACK_CROWBAR] != nullptr)
		{
			FlipbookInfo info;
			info.name = L"MobCharAnim_Crowbar_Idle_Auto";
			info.frames.push_back(_anims[(int)AnimType::ATTACK_CROWBAR]->GetInfo().frames[0]);
			info.duration = 1.0f;
			info.loop = true;
			_anims[(int)AnimType::IDLE_CROWBAR]->SetInfo(info);
		}

		_anims[(int)AnimType::IDLE_GUN] = FM->CreateFlipBook(L"MobCharAnim_Gun_Idle_Auto");
		if (_anims[(int)AnimType::IDLE_GUN]->GetInfo().frames.empty() && _anims[(int)AnimType::ATTACK_GUN] != nullptr)
		{
			FlipbookInfo info;
			info.name = L"MobCharAnim_Gun_Idle_Auto";
			info.frames.push_back(_anims[(int)AnimType::ATTACK_GUN]->GetInfo().frames[0]);
			info.duration = 1.0f;
			info.loop = true;
			_anims[(int)AnimType::IDLE_GUN]->SetInfo(info);
		}
	}
	
	{
		_Fanims[(int)AnimType::IDLE] = FM->GetFlipBook(L"FMobCharAnim_Idle");
		_Fanims[(int)AnimType::MOVE] = FM->GetFlipBook(L"FMobCharAnim_Walking");
		_Fanims[(int)AnimType::ATTACK_FIST] = FM->GetFlipBook(L"FMobCharAnim_Punch");
		_Fanims[(int)AnimType::EXECUTE] = FM->GetFlipBook(L"FMobCharAnim_Execution");
		_Fanims[(int)AnimType::DEAD] = FM->GetFlipBook(L"FMobCharAnim_Dead");
		_Fanims[(int)AnimType::UNCONSCIOUS] = FM->GetFlipBook(L"FMobCharAnim_Unconscious");
	}
	
	if (_enemyType != EnemyType::ARMORED)
	{
		switch (currentWeapon_Enemy)
		{
		case WPTYPE::FIST:
			PlayAnimation(_anims[(int)AnimType::IDLE]);
			break;
		case WPTYPE::CROWBAR:
			PlayAnimation(_anims[(int)AnimType::IDLE_CROWBAR]);
			break;
		case WPTYPE::RIFLE:
			PlayAnimation(_anims[(int)AnimType::IDLE_GUN]);
			break;
		default:
			break;
		}
	}
	else
		PlayAnimation(_Fanims[(int)AnimType::IDLE]);
}

void Enemy::Update()
{
	if (IsKilled())	return;

}

void Enemy::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);
}

bool Enemy::IsKilled()
{
	return _enemyState == EnemyState::DEAD;
}

void Enemy::OnCollision(GameObject* other)
{
	if (IsKilled()) return;

	switch (other->GetObjectType())
	{
	case OBJECTTYPE::PROJECTILE:
	{
		Projectile* Proj = static_cast<Projectile*>(other);
		if (Proj->IsLetal())
		{
			if (_enemyType == EnemyType::NORMAL)
			{
				_enemyState = EnemyState::DEAD;
				PlayAnimation(_anims[(int)AnimType::DEAD]);
			}
			else
			{
				if (_isHit)
					PlayAnimation(_Fanims[(int)AnimType::DEAD]);
				else
				{
					_isHit = true;
					_enemyState = EnemyState::UNCONSCIOUS;
					PlayAnimation(_Fanims[(int)AnimType::UNCONSCIOUS]);
				}
			}
		}
		else
		{
			_enemyState = EnemyState::UNCONSCIOUS;
			if (_enemyType == EnemyType::NORMAL)
			{
				PlayAnimation(_anims[(int)AnimType::UNCONSCIOUS]);
			}
			else
			{
				PlayAnimation(_Fanims[(int)AnimType::UNCONSCIOUS]);
			}
		}
	}
		break;
	default:
		break;
	}
}

void Enemy::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.enemyType = _enemyType;
	outData.weaponType = currentWeapon_Enemy;
}

void Enemy::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	_enemyType = spawnData.enemyType;
	currentWeapon_Enemy = spawnData.weaponType;
}

void Enemy::EmMove()
{
}

void Enemy::SetEnemyType(EnemyType etype)
{
	_enemyType = etype;
	switch (etype)
	{
	case EnemyType::NORMAL:
		speed = baseEnemySpeed;
		switch (currentWeapon_Enemy)
		{
		case WPTYPE::FIST:
			PlayAnimation(_anims[(int)AnimType::IDLE]);
			break;
		case WPTYPE::CROWBAR:
			PlayAnimation(_anims[(int)AnimType::IDLE_CROWBAR]);
			break;
		case WPTYPE::RIFLE:
			PlayAnimation(_anims[(int)AnimType::IDLE_GUN]);
			break;
		default:
			break;
		}
		break;
	case EnemyType::ARMORED:
		currentWeapon_Enemy = WPTYPE::FIST;
		_currentAmmo = -1;
		speed = armoredEnemySpeed;
		switch (currentWeapon_Enemy)
		{
		case WPTYPE::FIST:
			PlayAnimation(_Fanims[(int)AnimType::IDLE]);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	

}

void Enemy::SetWPTYPE(WPTYPE wType)
{
	if (_enemyType != EnemyType::NORMAL)
		return;
	switch (wType)
	{
	case WPTYPE::FIST:
		currentWeapon_Enemy = wType;
		_currentAmmo = GetWeaponInfo(currentWeapon_Enemy).maxAmmo;
		PlayAnimation(_anims[(int)AnimType::IDLE]);
		break;
	case WPTYPE::CROWBAR:
		currentWeapon_Enemy = wType;
		_currentAmmo = GetWeaponInfo(currentWeapon_Enemy).maxAmmo;
		PlayAnimation(_anims[(int)AnimType::IDLE_CROWBAR]);
		break;
	case WPTYPE::RIFLE:
		currentWeapon_Enemy = wType;
		_currentAmmo = GetWeaponInfo(currentWeapon_Enemy).maxAmmo;
		PlayAnimation(_anims[(int)AnimType::IDLE_GUN]);
		break;
	default:
		break;
	}
}

