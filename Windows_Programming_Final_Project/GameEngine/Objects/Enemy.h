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

	bool _projectileSpawned = false;
	float AC = 0.0f; //accumulator 몇초동안 기절해 있게 할건지
	FlipBook* _Fanims[(int)AnimType::TOTAL_COUNT] = { nullptr }; // 장갑인 놈 바꾸는거 할때마다 새로 할당하기 귀찮다

public:

	Enemy();
	Enemy(EnemyType type);
	virtual ~Enemy();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;
	virtual bool IsKilled() override;
	virtual void OnCollision(GameObject* other) override;
	

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;


	//적 이동 함수
	//보류
	void EmMove();



	EnemyType GetEType() { return _enemyType; }
	void SetEnemyType(EnemyType etype) { _enemyType = etype;  };

	WPTYPE GetWPTYPE() { return currentWeapon_Enemy; };
	void SetWPTYPE(WPTYPE wType) { currentWeapon_Enemy = wType; };

	EnemyState GetEnemyState() { return _enemyState; };
	void SetEnemyState(EnemyState state) { _enemyState = state; };


};