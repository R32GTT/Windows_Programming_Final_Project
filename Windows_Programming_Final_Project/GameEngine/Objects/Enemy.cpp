#include "pch.h"
#include "Enemy.h"
#include "Managers.h"
#include "Objects.h"
#include "../LevelData/LevelData.h"
#include "../FileBase/FileTypes/FlipBook.h"
#include "../Scene/Scene.h"
#include "Enums.h"


// === [추가] Enemy 정적 공유 변수 초기화 ===
Vec2F Enemy::s_playerPos = Vec2F(0.0f, 0.0f);
bool Enemy::s_isAlerted = false;

//다시 푸시하기

Enemy::Enemy()
{
	speed = baseEnemySpeed;
	_halfSize = Vec2F(30.0f, 15.0f);

	type = OBJECTTYPE::ENEMY;
	layer = Layers::ACTORS;
}

Enemy::Enemy(EnemyType etype)
{
	_halfSize = Vec2F(30.0f, 15.0f);
	type = OBJECTTYPE::ENEMY;
	layer = Layers::ACTORS;

	if (etype == EnemyType::NORMAL)
		speed = baseEnemySpeed;
	else
	{
		_halfSize = Vec2F(40.0f, 20.0f);
		speed = armoredEnemySpeed;
	}
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
	SavePrevPos();
	// 쿨타임 감소 로직 (살아있고 기절하지 않았을 때만)
	if (_enemyState != EnemyState::DEAD && _enemyState != EnemyState::UNCONSCIOUS)
	{
		if (_attackCooldown > 0.0f)
			_attackCooldown -= 1.0f / 60.0f;
	}

	// 1. 사망 체크
	if (_enemyState == EnemyState::DEAD)
	{
		FlipBook** animArray = (_enemyType == EnemyType::ARMORED) ? _Fanims : _anims;
		if (_currAnim != animArray[(int)AnimType::DEAD])
			PlayAnimation(animArray[(int)AnimType::DEAD]);

		UpdateAnimation(1.0f / 60.0f);
		return;
	}

	// 2. 🔥 기절 및 기상 로직 (AC 변수 활용)
	if (_enemyState == EnemyState::UNCONSCIOUS)
	{
		FlipBook** animArray = (_enemyType == EnemyType::ARMORED) ? _Fanims : _anims;
		if (_currAnim != animArray[(int)AnimType::UNCONSCIOUS])
			PlayAnimation(animArray[(int)AnimType::UNCONSCIOUS]);

		UpdateAnimation(1.0f / 60.0f);

		// 플레이어에게 처형(EXECUTE) 당하고 있는 중이 아닐 때만 시간이 흐름!
		if (!_onExecution)
		{
			AC += 1.0f / 60.0f; // 델타타임 누적 (초 단위)

			if (AC >= 5.0f) // 🔥 5초 뒤에 일어남 (원하시는 시간으로 조절하세요!)
			{
				_enemyState = EnemyState::IDLE; // 다시 IDLE 상태로 복귀
				AC = 0.0f;                      // 누적기 초기화
				_isHit = false;                 // 피격 판정도 초기화 (장갑병 등 대비)
			}
		}

		return; // 기절 중에는 밑의 AI(이동, 공격) 로직을 타지 않음
	}

	// 3. 플레이어 위치 갱신
	if (GET_SINGLE(SceneManager)->GetCurrentScene() && GET_SINGLE(SceneManager)->GetCurrentScene()->GetPlayer())
	{
		if (GET_SINGLE(SceneManager)->GetCurrentScene()->GetPlayer()->IsKilled()) // 플레이어 죽으면 그냥 더 연산 안함.
			return; 
		s_playerPos = GET_SINGLE(SceneManager)->GetCurrentScene()->GetPlayer()->GetPos();

	}
	// 3. Player와 완벽히 동일한 애니메이션 타겟 매핑
	AnimType targetIdle = AnimType::IDLE;
	AnimType targetMove = AnimType::MOVE;
	AnimType targetAttack = AnimType::ATTACK_FIST;

	if (_enemyType == EnemyType::NORMAL)
	{
		if (currentWeapon_Enemy == WPTYPE::CROWBAR)
		{
			targetIdle = AnimType::IDLE_CROWBAR;
			targetMove = AnimType::IDLE_CROWBAR;
			targetAttack = AnimType::ATTACK_CROWBAR;
		}
		else if (currentWeapon_Enemy == WPTYPE::RIFLE)
		{
			targetIdle = AnimType::IDLE_GUN;
			targetMove = AnimType::IDLE_GUN;
			targetAttack = AnimType::ATTACK_GUN;
		}
	}

	FlipBook** animsToUse = (_enemyType == EnemyType::ARMORED) ? _Fanims : _anims;

	// 4. 상태 머신
	switch (_enemyState)
	{
	case EnemyState::IDLE:
	case EnemyState::PATROL:
	case EnemyState::CHASE:
	{
		Vec2F dirToPlayer = s_playerPos - pos;
		float distToPlayer = dirToPlayer.Length();
		float attackRange = 80.0f; // 기본 (빠루 등)
		if (currentWeapon_Enemy == WPTYPE::RIFLE)
			attackRange = 300.0f;  // 총은 멀리서
		else if (currentWeapon_Enemy == WPTYPE::FIST)
			attackRange = 50.0f;   // 주먹은 아주 가까이 와야 공격 시작!

		// --- 공격 사거리 진입 시 ---
		if (_enemyState == EnemyState::CHASE && distToPlayer <= attackRange)
		{
			if (_attackCooldown <= 0.0f)
			{
				_enemyState = EnemyState::ATTACK;
				_attackHitCount = 0;
				_burstCount = 0; // 점사 카운트 초기화

				_currFrame = 0;
				_animTimer = 0.0f;
				break;
			}
		}

		// --- 이동 로직 ---
		if (_enemyState == EnemyState::CHASE)
		{
			if (distToPlayer > _viewDistance * 1.5f)
			{
				_enemyState = EnemyState::PATROL;
				_isStartPosSet = false;
			}
			else if (distToPlayer > 0.0f)
			{
				facingDir = dirToPlayer.Normalized();
				_rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f;
				float stopDist = (currentWeapon_Enemy == WPTYPE::RIFLE) ? 150.0f : 20.0f;
				if (distToPlayer > stopDist) {
					pos = pos + facingDir * speed;
				}
			}
		}
		else if (_enemyState == EnemyState::PATROL)
		{
			if (CheckPlayerInSight())
			{
				_enemyState = EnemyState::CHASE;
				break;
			}
			if (!_isStartPosSet)
			{
				_startPos = pos;
				_isStartPosSet = true;
				if (_moveDir.LengthSq() == 0.0f) _moveDir = Vec2F(1.0f, 0.0f);
			}
			facingDir = _moveDir;
			_rotationAngle = _moveDir.Angle() * (180.0f / PI) + 90.0f;
			pos = pos + _moveDir * speed;

			if ((pos - _startPos).Length() >= _patrolRange)
			{
				_moveDir = _moveDir * -1.0f;
				_startPos = pos;
			}
		}
		else if (_enemyState == EnemyState::IDLE)
		{
			_enemyState = EnemyState::PATROL;
		}

		if (_enemyState == EnemyState::CHASE || _enemyState == EnemyState::PATROL)
		{
			if (_currAnim != animsToUse[(int)targetMove])
				PlayAnimation(animsToUse[(int)targetMove]);
		}
		else
		{
			if (_currAnim != animsToUse[(int)targetIdle])
				PlayAnimation(animsToUse[(int)targetIdle]);
		}
		break;
	}

	case EnemyState::ATTACK:
	{
		if (_currAnim != animsToUse[(int)targetAttack])
		{
			PlayAnimation(animsToUse[(int)targetAttack]);
		}

		// 조준 유지
		Vec2F dirToPlayer = s_playerPos - pos;
		if (dirToPlayer.LengthSq() > 0.0f)
		{
			facingDir = dirToPlayer.Normalized();
			_rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f;
		}

		// 타격 판정 (Fire)
		const WeaponInfo& weaponInfo = GetWeaponInfo(currentWeapon_Enemy);
		if (_attackHitCount < weaponInfo.attackFrames.size())
		{
			int targetFrame = weaponInfo.attackFrames[_attackHitCount];
			if (_currFrame == targetFrame)
			{
				Fire();
				_attackHitCount++;
			}
		}

		// 🔥 [핵심 추가] 점사 및 쿨타임 로직
		if (_currAnim != nullptr && _currAnim == animsToUse[(int)targetAttack])
		{
			int maxFrame = _currAnim->GetInfo().frames.size() - 1;
			if (_currFrame >= maxFrame)
			{
				if (currentWeapon_Enemy == WPTYPE::RIFLE)
				{
					_burstCount++;
					if (_burstCount >= 3) // 🔥 3번 쐈으면(3점사) 멈춤!
					{
						_attackCooldown = 1.5f; // 1.5초 동안 대기
						_enemyState = EnemyState::CHASE;
					}
					else
					{
						// 아직 3발을 덜 쐈다면 애니메이션을 즉시 0으로 되돌려 바로 다음 탄 발사 (연사)
						_currFrame = 0;
						_animTimer = 0.0f;
						_attackHitCount = 0;
					}
				}
				else
				{
					// 근접 무기(FIST, CROWBAR)의 경우도 헛스윙 무한 반복을 막기 위해 0.5초 딜레이
					_attackCooldown = 0.5f;
					_enemyState = EnemyState::CHASE;
				}
			}
		}
		break;
	}
	}

	// 실제 애니메이션 프레임 진행
	UpdateAnimation(1.0f / 60.0f);
}

void Enemy::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;
	if (_onExecution) return;

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
	case OBJECTTYPE::WALL:
		if (_enemyState == EnemyState::PATROL)
		{
			_moveDir = _moveDir * -1.0f;
			facingDir = _moveDir;
			_rotationAngle = _moveDir.Angle() * (180.0f / PI) + 90.0f;


			_startPos = pos;
		}

		else
		{

			Vec2F leftDir(-facingDir.y, facingDir.x);


			if (leftDir.LengthSq() > 0.0f)
			{
				leftDir = leftDir.Normalized();
				facingDir = leftDir;
				_rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f;
				pos = pos + leftDir * (speed * 1.5f);
			}
		}
		break;
	default:
		break;
	}
}


// 1. 기존의 매개변수 2개짜리 함수 (부모 클래스와의 약속)
// 이제 이 함수는 직접 로직을 처리하지 않고, 3개짜리 함수로 토스만 합니다.
void Enemy::OnHit_Recoil(bool isLethal, Vec2F dir)
{
	OnHit_Recoil(isLethal, dir, WPTYPE::NONE);
}

// 2. [새로 추가된] 매개변수 3개짜리 함수 (실제 모든 로직 처리)
void Enemy::OnHit_Recoil(bool isLethal, Vec2F dir, WPTYPE hitWeapon)
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

			// ===================================================
			// [추가] 노말 적 사망 시 점수 계산!
			GET_SINGLE(SceneManager)->OnEnemyKilled(hitWeapon);
			// ===================================================
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

				// ===================================================
				// [추가] 아머드 적 최종 사망 시 점수 계산!
				GET_SINGLE(SceneManager)->OnEnemyKilled(hitWeapon);
				// ===================================================
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

void Enemy::Fire()
{
	// 탄약 체크 (-1은 무한대)
	if (_currentAmmo == 0) return;
	if (_currentAmmo > 0) _currentAmmo--;

	// 투사체(Projectile) 생성 및 초기화
	Projectile* proj = new Projectile(this, currentWeapon_Enemy);
	proj->Init();

	GET_SINGLE(SceneManager)->GetCurrentScene()->AddObject(proj);	
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
	switch (_enemyType)
	{
	case EnemyType::NORMAL:
		speed = baseEnemySpeed;
		break;
	case EnemyType::ARMORED:
		speed = armoredEnemySpeed;
		break;
	}
	currentWeapon_Enemy = spawnData.weaponType;
	_currentAmmo = GetWeaponInfo(spawnData.weaponType).maxAmmo;

	// 추가: 데이터가 로드되어 실제 위치(_pos)가 결정된 순간을 시작 위치로 기록
	_startPos = pos;
	_targetPos = _startPos + (_moveDir * _patrolRange);
	_isStartPosSet = true;

	
}

void Enemy::EmMove()
{
	// 1. 최초 1회 시작 위치와 초기 방향 설정
	if (!_isStartPosSet)
	{
		_startPos = pos;
		_isStartPosSet = true;

		if (_moveDir.LengthSq() == 0.0f) _moveDir = Vec2F(1.0f, 0.0f);


		_rotationAngle = _moveDir.Angle() * (180.0f / PI) + 90.0f;
	}


	pos = pos + _moveDir * speed;


	if (_enemyType == EnemyType::NORMAL)
		PlayAnimation(_anims[(int)AnimType::MOVE]);
	else
		PlayAnimation(_Fanims[(int)AnimType::MOVE]);


	float distFromStart = (pos - _startPos).Length();
	if (distFromStart >= _patrolRange)
	{
		_moveDir = _moveDir * -1.0f;

		_rotationAngle = _moveDir.Angle() * (180.0f / PI) + 90.0f;


		_startPos = pos;
	}
}

// === [추가] 시야 체크 함수 구현 (Enemy.cpp 맨 아래에 배치) ===
bool Enemy::CheckPlayerInSight()
{
	Vec2F dirToPlayer = s_playerPos - pos;
	float dist = dirToPlayer.Length();

	if (dist > _viewDistance) return false;


	float closeSenseRange = 60.0f; // 캐릭터 덩치에 맞춰 조절하세요 (반지름의 2배 정도)
	if (dist <= closeSenseRange)
	{
		s_isAlerted = true;
		return true;
	}

	float rad = (_rotationAngle - 90.0f) * (PI / 180.0f);
	Vec2F forwardDir = Vec2F(cos(rad), sin(rad));
	Vec2F normDirToPlayer = dirToPlayer.Normalized();

	float dotProduct = forwardDir.Dot(normDirToPlayer);
	if (dotProduct >= cos(_viewAngle * PI / 180.0f))
	{
		s_isAlerted = true;
		return true;
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


