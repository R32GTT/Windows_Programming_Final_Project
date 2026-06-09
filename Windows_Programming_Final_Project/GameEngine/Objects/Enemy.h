#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Enemy : public GameObject {

private:
	//어떤 무기를 가지고 있는지 변수(적용)
	WPTYPE currentWeapon_Enemy = WPTYPE::NONE;

	EnemyType _enemyType = EnemyType::NORMAL;
	EnemyState _enemyState = EnemyState::IDLE; // 상태는 이걸로 체크

public:

	Enemy();
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

	void SetEnemyType(EnemyType etype);

	//일반적인지 덩치인지 구분하는 함수
	//? GetEnemyType 만들면 더 편하지 않을까?
	EnemyType What_Enemy();
	EnemyType GetEType();

	EnemyState GetEnemyState() { return _enemyState; };
	void SetEnemyState(EnemyState state) { _enemyState = state; };


};