#include "pch.h"
#include "Enemy.h"
#include "Managers.h"
#include "Objects.h"
#include "../LevelData/LevelData.h"
#include "../FileBase/FileTypes/FlipBook.h"

// === [추가] Enemy 정적 공유 변수 초기화 ===
Vec2F Enemy::s_playerPos = Vec2F(0.0f, 0.0f);
bool Enemy::s_isAlerted = false;


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

//간단한 적 Ai를 구현함
void Enemy::Update()
{
	SavePrevPos();

	// 행동 불능 상태가 아닐 때만 패턴을 수행하도록 제어
	if (_enemyState != EnemyState::DEAD && _enemyState != EnemyState::UNCONSCIOUS)
	{
		if (_enemyState == EnemyState::IDLE)
		{
			_enemyState = EnemyState::PATROL;
		}

		if (_enemyState == EnemyState::PATROL)
		{
			EmMove();
		}
	}
	
	GameObject::Update();

}

void Enemy::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);

	// === [추가] AI 디버그 렌더링 (시야에 들어와 추적 중일 때 붉은 선 그리기) ===
	/*if (_enemyState == EnemyState::PATROL)
	{
		ID2D1SolidColorBrush* debugBrush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);

		if (debugBrush)
		{
			// 플레이어의 화면 상 렌더링 좌표 계산
			Vec2F playerScreenPos = GET_SINGLE(SceneManager)->ToRenderPos(s_playerPos);

			// 적 위치에서 플레이어 위치까지 선 긋기
			renderTarget->DrawLine(
				D2D1::Point2F(ToRenderPos.x, ToRenderPos.y),
				D2D1::Point2F(playerScreenPos.x, playerScreenPos.y),
				debugBrush,
				2.0f // 선 두께
			);

			debugBrush->Release();
		}
	}*/

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
		OnHit_Recoil(Proj->IsLethal(), Proj->GetDir());
	}
		break;
	default:
		break;
	}
}

void Enemy::OnHit_Recoil(bool isLethal, Vec2F dir)
{
	switch (_enemyType)
	{
	case EnemyType::NORMAL:
	{
		if (isLethal)
		{
			_enemyState = EnemyState::DEAD;
			_rotationAngle = dir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
			PlayAnimation(_anims[(int)AnimType::DEAD]);
		}
		else
		{
			_enemyState = EnemyState::UNCONSCIOUS;
			_rotationAngle = dir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
			PlayAnimation(_anims[(int)AnimType::UNCONSCIOUS]);
		}
	}
	break;
	case EnemyType::ARMORED:
	{
		if (isLethal) //최소 한대는 맞았어야 함
		{
			if (_isHit)
			{
				_enemyState = EnemyState::DEAD;
				_rotationAngle = dir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
				PlayAnimation(_Fanims[(int)AnimType::DEAD]);
			}
			else
			{
				_isHit = true;
				_enemyState = EnemyState::UNCONSCIOUS;
				_rotationAngle = dir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
				PlayAnimation(_Fanims[(int)AnimType::UNCONSCIOUS]);
			}
		}
		else
		{
			_isHit = true;
			_enemyState = EnemyState::UNCONSCIOUS;
			_rotationAngle = dir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
			PlayAnimation(_Fanims[(int)AnimType::UNCONSCIOUS]);
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

	// 추가: 데이터가 로드되어 실제 위치(_pos)가 결정된 순간을 시작 위치로 기록
	_startPos = pos;
	_targetPos = _startPos + (_moveDir * _patrolRange);
	_isStartPosSet = true;


}

void Enemy::EmMove()
{
	SavePrevPos();

	if (CheckPlayerInSight())
	{
		// === [추적 로직] 플레이어가 시야에 있을 때 ===
		Vec2F dirToPlayer = s_playerPos - pos;

		if (dirToPlayer.LengthSq() > 0.0f)
		{
			Vec2F moveDir = dirToPlayer.Normalized();

			// 1. 플레이어를 향해 회전각 업데이트 (OnHit_Recoil과 동일한 방식 적용)
			_rotationAngle = moveDir.Angle() * (180.0f / PI) + 90.0f;

			// 2. 플레이어 방향으로 이동
			pos = pos + moveDir * speed;

			// 3. 걷기 애니메이션 재생
			// (주의: 매 프레임 PlayAnimation이 호출되어 애니메이션이 0프레임으로 고정되는 현상이 있다면, 
			// 현재 재생 중인 애니메이션과 다를 때만 PlayAnimation을 호출하도록 예외 처리가 필요합니다.)
			if (_enemyType == EnemyType::NORMAL)
			{
				PlayAnimation(_anims[(int)AnimType::MOVE]);
			}
			else
			{
				PlayAnimation(_Fanims[(int)AnimType::MOVE]);
			}
		}
	}
	else
	{
		// === [기존 패트롤 로직] 플레이어를 발견하지 못했을 때 ===
		if (_isStartPosSet)
		{
			// TODO: _startPos와 _targetPos 사이를 왔다 갔다 하는 정찰 로직 구현부

			// 예시: 목적지에 도달했는지 확인하고 방향을 뒤집는 로직
			// Vec2F dirToTarget = _targetPos - pos;
			// ... (기존에 구상하신 패트롤 로직을 여기에 작성하시면 됩니다) ...

			// 임시로 기본 IDLE 애니메이션 유지
			if (_enemyType == EnemyType::NORMAL)
				PlayAnimation(_anims[(int)AnimType::IDLE]);
			else
				PlayAnimation(_Fanims[(int)AnimType::IDLE]);
		}
	}

	
}

// === [추가] 시야 체크 함수 구현 (Enemy.cpp 맨 아래에 배치) ===
bool Enemy::CheckPlayerInSight()
{
	Vec2F dirToPlayer = s_playerPos - pos;
	float dist = dirToPlayer.Length();

	// 1. 플레이어가 시야 거리 범위 안에 들어왔는지 확인
	if (dist <= _viewDistance)
	{
		// 2. 적이 바라보는 정면 방향 벡터 계산
		// (Sprite가 위를 향하기 때문에 +90 보정했던 것을 다시 -90 빼서 라디안으로 변환)
		float rad = (_rotationAngle - 90.0f) * (PI / 180.0f);
		Vec2F forwardDir = Vec2F(cos(rad), sin(rad));

		// 3. 플레이어를 향하는 방향 벡터 정규화
		Vec2F normDirToPlayer = dirToPlayer.Normalized();

		// 4. 내적(Dot Product)을 이용해 시야각 내에 있는지 확인
		// cos(45도) 값을 기준(약 0.707)으로 삼으면 정면 기준 좌우 45도(총 90도)의 시야각이 됩니다.
		float dotProduct = forwardDir.Dot(normDirToPlayer);

		if (dotProduct >= cos(45.0f * PI / 180.0f))
		{
			s_isAlerted = true; // [선택] 인식 상태 공유
			return true;        // 감지 성공!
		}
	}

	return false;
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

