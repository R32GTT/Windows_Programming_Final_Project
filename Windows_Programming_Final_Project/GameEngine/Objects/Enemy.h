#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Enemy : public GameObject {

private:
	//어떤 무기를 가지고 있는지 변수(적용)
	WPTYPE currentWeapon_Enemy = WPTYPE::FIST;

	EnemyType _enemyType = EnemyType::NORMAL;
	EnemyState _enemyState = EnemyState::IDLE; // 상태는 이걸로 체크

	int _currentAmmo{};
	int _attackHitCount = 0; // 공격 프레임 중복 방지용 카운트 (Player와 동일)

	bool _projectileSpawned = false;
	bool _isHit = false; // 장갑용
	float AC = 0.0f; //accumulator 몇초동안 기절해 있게 할건지
	FlipBook* _Fanims[(int)AnimType::TOTAL_COUNT] = { nullptr }; // 장갑인 놈 바꾸는거 할때마다 새로 할당하기 귀찮다

	//적들 순찰(왕복이동)을 위해 추가된 변수들
	Vec2F _startPos{};
	Vec2F _moveDir = Vec2F(1.f, 0.f); // 이동 방향 (기본값: 우측 X축 이동)
	float _patrolRange = 200.f;   // 왕복할 총 거리 (픽셀 단위)
	bool _isStartPosSet = false;  // 시작 위치가 올바르게 잡혔는지 체크하는 플래그
	bool _onExecution = false;

	// === [추가] 시야 감지 및 추적을 위한 세팅 ===
	float _viewDistance = 400.0f; // 적의 시야 거리 (픽셀 단위, 조절 가능)
	float _viewAngle = 60.0f;     // 적의 시야 반각 (정면 기준 좌우 60도, 총 120도 시야각)

	float _attackCooldown = 0.0f; 
	int _burstCount = 0;          
	Vec2F _velocity{};

	bool HasLineOfSightToPlayer();

	bool CheckPlayerInSight();    // 플레이어가 시야 내에 있는지 확인하는 함수

	Vec2F _targetPos{};


public:

	Enemy();
	Enemy(EnemyType type);
	virtual ~Enemy();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;
	virtual bool IsKilled() override;
	virtual void OnCollision(GameObject* other) override;

	void OnExecution(bool t) { _onExecution = t; };

	virtual void OnHit_Recoil(bool isLethal, Vec2F dir) override;

	//무기 정보(WPTYPE)를 추가로 받는 새로운 전용 함수 선언
	void OnHit_Recoil(bool isLethal, Vec2F dir, WPTYPE hitWeapon);
	virtual void Fire() override;

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;


	//적 이동 함수
	void EmMove();

	static void SetPlayerPos(Vec2F pos) { s_playerPos = pos; }

	EnemyType GetEType() { return _enemyType; }
	void SetEnemyType(EnemyType etype);

	WPTYPE GetWPTYPE() { return currentWeapon_Enemy; };
	void SetWPTYPE(WPTYPE wType);

	EnemyState GetEnemyState() { return _enemyState; };
	void SetEnemyState(EnemyState state) { _enemyState = state; };

	// [이전 추가] 모든 적이 공유할 정적(static) 멤버 변수
	static bool s_isAlerted;      // 한 명이라도 플레이어를 감지했는지 여부 (경보 플래그)
	static Vec2F s_playerPos;     // 플레이어의 실시간 위치 좌표

};