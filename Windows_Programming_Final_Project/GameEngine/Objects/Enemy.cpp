#include "pch.h"
#include "Enemy.h"
#include "Managers.h"
#include "Objects.h"
#include "../LevelData/LevelData.h"
#include "../FileBase/FileTypes/FlipBook.h"
#include "../Scene/Scene.h"
#include "Enums.h"
#include <random>


Vec2F Enemy::s_playerPos = Vec2F(0.0f, 0.0f);
bool Enemy::s_isAlerted = false;



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
		case WPTYPE::RANDOM_ANY:
			PlayAnimation(_anims[(int)AnimType::IDLE]);
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

	if (_enemyState != EnemyState::DEAD && _enemyState != EnemyState::UNCONSCIOUS)
	{
		if (_attackCooldown > 0.0f) _attackCooldown -= 1.0f / 60.0f;
	}

	if (_enemyState == EnemyState::DEAD)
	{
		_velocity = Vec2F(0.0f, 0.0f);
		FlipBook** animArray = (_enemyType == EnemyType::ARMORED) ? _Fanims : _anims;
		if (_currAnim != animArray[(int)AnimType::DEAD])
			PlayAnimation(animArray[(int)AnimType::DEAD]);

		UpdateAnimation(1.0f / 60.0f);
		return;
	}

	if (_enemyState == EnemyState::UNCONSCIOUS)
	{
		_velocity = Vec2F(0.0f, 0.0f);
		FlipBook** animArray = (_enemyType == EnemyType::ARMORED) ? _Fanims : _anims;
		if (_currAnim != animArray[(int)AnimType::UNCONSCIOUS])
			PlayAnimation(animArray[(int)AnimType::UNCONSCIOUS]);

		if (!_onExecution)
		{
			AC += 1.0f / 60.0f;
			if (AC >= 5.0f)
			{
				_enemyState = EnemyState::IDLE;
				AC = 0.0f;
				_isHit = false;
			}
		}

		if (_enemyState == EnemyState::UNCONSCIOUS)
		{
			UpdateAnimation(1.0f / 60.0f);
			return;
		}
	}

	if (GET_SINGLE(SceneManager)->GetCurrentScene() && GET_SINGLE(SceneManager)->GetCurrentScene()->GetPlayer())
	{
		s_playerPos = GET_SINGLE(SceneManager)->GetCurrentScene()->GetPlayer()->GetPos();
	}

	AnimType targetIdle = AnimType::IDLE;
	AnimType targetMove = AnimType::MOVE;
	AnimType targetAttack = AnimType::ATTACK_FIST;

	if (_enemyType == EnemyType::NORMAL)
	{
		if (currentWeapon_Enemy == WPTYPE::CROWBAR) {
			targetIdle = AnimType::IDLE_CROWBAR;
			targetMove = AnimType::IDLE_CROWBAR;
			targetAttack = AnimType::ATTACK_CROWBAR;
		}
		else if (currentWeapon_Enemy == WPTYPE::RIFLE) {
			targetIdle = AnimType::IDLE_GUN;
			targetMove = AnimType::IDLE_GUN;
			targetAttack = AnimType::ATTACK_GUN;
		}
	}
	FlipBook** animsToUse = (_enemyType == EnemyType::ARMORED) ? _Fanims : _anims;

	switch (_enemyState)
	{
	case EnemyState::IDLE:
	{
		_velocity.x *= 0.8f;
		_velocity.y *= 0.8f;
		pos = pos + _velocity;

		if (CheckPlayerInSight())
		{
			_reactionTimer += 1.0f / 60.0f;
			if (_reactionTimer >= 0.15f)
			{
				_enemyState = EnemyState::CHASE;
				_reactionTimer = 0.0f;
			}
		}
		else
		{
			_reactionTimer = 0.0f;
		}
		break;
	}
	case EnemyState::ROAMING:
	{
		if (CheckPlayerInSight())
		{
			_reactionTimer += 1.0f / 60.0f;
			if (_reactionTimer >= 0.15f) 
			{
				_enemyState = EnemyState::CHASE;
				_reactionTimer = 0.0f;
				break;
			}
		}
		else
		{
			_reactionTimer = 0.0f;

			if (!_isStartPosSet)
			{
				_startPos = pos;
				_targetPos = pos;
				_isStartPosSet = true;
				_roamTimer = -1.0f;
			}

			Vec2F dirToTarget = _targetPos - pos;
			float distToTarget = dirToTarget.Length();

			if (distToTarget <= 20.0f || _roamTimer <= 0.0f)
			{
				_velocity.x *= 0.8f;
				_velocity.y *= 0.8f;
				_roamTimer -= 1.0f / 60.0f;

				if (_roamTimer <= -1.0f)
				{
					float randAngle = (rand() % 360) * (3.141592f / 180.0f);
					float randDist = 50.0f + (rand() % 100);

					_targetPos.x = _startPos.x + cos(randAngle) * randDist;
					_targetPos.y = _startPos.y + sin(randAngle) * randDist;

					_roamTimer = 3.0f;
				}
			}
			else
			{
				_roamTimer -= 1.0f / 60.0f;

				_moveDir = dirToTarget.Normalized();
				facingDir = _moveDir;

				_rotationAngle = _moveDir.Angle() * (180.0f / 3.141592f) + 90.0f;

				Vec2F desiredVelocity = _moveDir * (speed * 0.5f);
				_velocity.x += (desiredVelocity.x - _velocity.x) * 0.1f;
				_velocity.y += (desiredVelocity.y - _velocity.y) * 0.1f;
			}

			pos = pos + _velocity;
		}
		break;
	}
	case EnemyState::PATROL:
	{
		if (CheckPlayerInSight())
		{
			_reactionTimer += 1.0f / 60.0f;
			if (_reactionTimer >= 0.15f)
			{
				_enemyState = EnemyState::CHASE;
				_reactionTimer = 0.0f;
				break;
			}
		}
		else
		{
			_reactionTimer = 0.0f;
		}

		if (!_isStartPosSet)
		{
			_startPos = pos;
			_isStartPosSet = true;
			if (_moveDir.LengthSq() == 0.0f) _moveDir = Vec2F(1.0f, 0.0f);
		}

		facingDir = _moveDir;
		_rotationAngle = _moveDir.Angle() * (180.0f / PI) + 90.0f;

		Vec2F desiredVelocity = _moveDir * speed;
		_velocity.x += (desiredVelocity.x - _velocity.x) * 0.1f;
		_velocity.y += (desiredVelocity.y - _velocity.y) * 0.1f;
		pos = pos + _velocity;

		if ((pos - _startPos).Length() >= _patrolRange)
		{
			_moveDir = _moveDir * -1.0f;
			_startPos = pos;
		}
		break;
	}
	case EnemyState::CHASE:
	{
		bool canSeePlayer = CheckPlayerInSight();

		if (canSeePlayer)
		{
			_targetPos = s_playerPos;
		}

		Vec2F dirToTarget = _targetPos - pos;
		float distToTarget = dirToTarget.Length();

		if (!canSeePlayer && distToTarget <= 20.0f)
		{
			_enemyState = _defaultState;
			_isStartPosSet = false;
			break;
		}

		float attackRange = 80.0f;
		float stopDist = 20.0f;

		if (currentWeapon_Enemy == WPTYPE::RIFLE) {
			attackRange = 300.0f;
			stopDist = 150.0f;
		}
		else if (currentWeapon_Enemy == WPTYPE::FIST) {
			attackRange = 50.0f;
			stopDist = 5.0f;
		}


		if (canSeePlayer && distToTarget <= attackRange && _attackCooldown <= 0.0f)
		{
			_reactionTimer += 1.0f / 60.0f;

			_velocity.x *= 0.5f;
			_velocity.y *= 0.5f;
			pos = pos + _velocity;

			if (_reactionTimer >= 0.25f) 
			{
				_enemyState = EnemyState::ATTACK;
				_attackHitCount = 0;
				_burstCount = 0;
				_currFrame = 0;
				_animTimer = 0.0f;
				_velocity = Vec2F(0.0f, 0.0f);
				_reactionTimer = 0.0f; 
			}
			break;
		}
		else
		{
			_reactionTimer = 0.0f;
		}


		if (distToTarget > _viewDistance * 1.5f)
		{
			_enemyState = _defaultState;
			_isStartPosSet = false;
			break;
		}

		if (distToTarget > 0.0f)
		{
			facingDir = dirToTarget.Normalized();
			_rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f;

			Vec2F desiredVel(0.0f, 0.0f);
			if (distToTarget > stopDist) {
				desiredVel = facingDir * speed;
			}

			_velocity.x += (desiredVel.x - _velocity.x) * 0.1f;
			_velocity.y += (desiredVel.y - _velocity.y) * 0.1f;
			pos = pos + _velocity;
		}
		break;
	}
	case EnemyState::ATTACK:
	{
		_velocity = Vec2F(0.0f, 0.0f);

		Vec2F dirToPlayer = s_playerPos - pos;
		if (dirToPlayer.LengthSq() > 0.0f)
		{
			facingDir = dirToPlayer.Normalized();
			_rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f;
		}

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
		break;
	}
	}

	if (_enemyState == EnemyState::IDLE)
	{
		if (_currAnim != animsToUse[(int)targetIdle]) PlayAnimation(animsToUse[(int)targetIdle]);
	}
	else if (_enemyState == EnemyState::PATROL || _enemyState == EnemyState::CHASE)
	{
		if (_currAnim != animsToUse[(int)targetMove]) PlayAnimation(animsToUse[(int)targetMove]);
	}
	else if (_enemyState == EnemyState::ATTACK)
	{
		if (_currAnim != animsToUse[(int)targetAttack]) PlayAnimation(animsToUse[(int)targetAttack]);

		if (_currAnim != nullptr && _currAnim == animsToUse[(int)targetAttack])
		{
			int maxFrame = _currAnim->GetInfo().frames.size() - 1;
			if (_currFrame >= maxFrame)
			{
				if (currentWeapon_Enemy == WPTYPE::RIFLE)
				{
					_burstCount++;
					if (_burstCount >= 3) {
						_attackCooldown = 1.5f;
						_enemyState = EnemyState::CHASE;
					}
					else {
						_currFrame = 0;
						_animTimer = 0.0f;
						_attackHitCount = 0;
					}
				}
				else
				{
					_attackCooldown = 0.5f;
					_enemyState = EnemyState::CHASE;
				}
			}
		}
	}

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
		DropWeapon();
		OnHit_Recoil(Proj->IsLethal(), Proj->GetDir(), Proj->GetWeaponType());
	}
	break;
	case OBJECTTYPE::WALL:
	{
		if (_enemyState == EnemyState::PATROL)
		{
			_moveDir = _moveDir * -1.0f;
			facingDir = _moveDir;
			_rotationAngle = _moveDir.Angle() * (180.0f / PI) + 90.0f;
			_velocity = Vec2F(0.0f, 0.0f);
			_startPos = pos; // 매니저가 밀어준 안전한 위치를 새 출발점으로
		}
		else
		{
			float diffX = std::abs(pos.x - other->GetPos().x);
			float diffY = std::abs(pos.y - other->GetPos().y);
			float minX = _halfSize.x + other->GetHalfSize().x;
			float minY = _halfSize.y + other->GetHalfSize().y;

			float overlapX = minX - diffX;
			float overlapY = minY - diffY;

			if (overlapX < overlapY)
			{
				_velocity.x = 0.0f; // 좌/우 벽에 부딪히면 좌우 이동 관성만 소멸 -> 상하로 미끄러짐
			}
			else
			{
				_velocity.y = 0.0f; // 상/하 벽에 부딪히면 상하 이동 관성만 소멸 -> 좌우로 미끄러짐
			}
		}
	}
		break;
	default:
		break;
	}
}


void Enemy::DropWeapon()
{
	if (currentWeapon_Enemy == WPTYPE::FIST || currentWeapon_Enemy == WPTYPE::NONE) return;
	Weapon* wep = new Weapon();
	wep->Init();
	wep->SetWeaponType(currentWeapon_Enemy);
	wep->SetAmmo(_currentAmmo);
	wep->SetPos(pos);
	wep->SavePrevPos();
	GET_SINGLE(SceneManager)->GetCurrentScene()->AddObject(wep);

	currentWeapon_Enemy = WPTYPE::FIST;
	_currentAmmo = -1;
}


void Enemy::OnHit_Recoil(bool isLethal, Vec2F dir)
{
	OnHit_Recoil(isLethal, dir, WPTYPE::NONE);
}


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

			GET_SINGLE(SceneManager)->OnEnemyKilled(hitWeapon);

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

				GET_SINGLE(SceneManager)->OnEnemyKilled(hitWeapon);

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
	if (_currentAmmo == 0) return;
	if (_currentAmmo > 0) _currentAmmo--;

	Projectile* proj = new Projectile(this, currentWeapon_Enemy);
	proj->Init();

	GET_SINGLE(SceneManager)->GetCurrentScene()->AddObject(proj);	
}


void Enemy::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.enemyType = _enemyType;
	outData.weaponType = currentWeapon_Enemy;
	outData.enemyState = _defaultState;
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

	SetEnemyState(spawnData.enemyState);

	WPTYPE loadedWeapon = spawnData.weaponType;
	if (spawnData.weaponType == WPTYPE::RANDOM_ANY && _enemyType != EnemyType::ARMORED && GET_SINGLE(SceneManager)->GetCurrentSceneType() != SceneType::EDITSCENE)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());

		std::uniform_int_distribution<int> dist(0, 2);

		int randVal = dist(gen);
		if (randVal == 0) loadedWeapon = WPTYPE::FIST;
		else if (randVal == 1) loadedWeapon = WPTYPE::CROWBAR;
		else if (randVal == 2) loadedWeapon = WPTYPE::RIFLE;
	}
	currentWeapon_Enemy = loadedWeapon;
	_currentAmmo = GetWeaponInfo(loadedWeapon).maxAmmo;

	_startPos = pos;
	_targetPos = _startPos + (_moveDir * _patrolRange);
	_isStartPosSet = true;
}

void Enemy::EmMove()
{
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

bool Enemy::CheckPlayerInSight()
{
	Vec2F dirToPlayer = s_playerPos - pos;
	float dist = dirToPlayer.Length();

	if (currentWeapon_Enemy == WPTYPE::RIFLE)
		_viewDistance = 900.0f;
	if (dist > _viewDistance) return false;

	if (!HasLineOfSightToPlayer()) return false;

	float closeSenseRange = 60.0f;
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
	case WPTYPE::RANDOM_ANY:
		currentWeapon_Enemy = wType;
		PlayAnimation(_anims[(int)AnimType::IDLE]);
	default:
		break;
	}
}


bool Enemy::HasLineOfSightToPlayer()
{
	return GET_SINGLE(CollisionManager)->CheckLineOfSight(pos, s_playerPos);
}