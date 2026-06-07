#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Projectile : public GameObject
{
public:

	enum class ProjectileType
	{
		BULLET,
		MELEE
	};

	Projectile();
	virtual ~Projectile();

	//Projectile 만들기 시작
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;
	virtual bool CheckDead() override;

	void SetDirection(Vec2<float> dir);

	//데미지는 한 방 컷이어서 일단 뺌
	//필요하면 추가하겠음 
	//void SetDamage(int damage);
	
	//owner의 의미는 이 Projectile을 누가 발사했는지를
	//기억하는 포인터이다
	void SetOwner(GameObject* owner);

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;


	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;
	virtual bool CheckDead() override;


	void SetInfo(GameObject* owner, ProjectileType type, float damage, float speed, float lifeTime);

	GameObject* GetOwner() { return _owner; }
	float GetDamage() { return _damage; }
	ProjectileType GetProjType() { return _projType; }

private:
	GameObject* _owner;         // 누가 발사/공격했는가 (자신이 맞지 않기 위해)
	ProjectileType _projType;   // 총알인지 근접인지

	float _damage;              // 공격력
	float _lifeTime;            // 최대 생존 시간 (초)
	float _currentTimer;        // 현재 생존 시간 계산용

	bool _isDead;               // 삭제 대기 상태인지 확인
};
