#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Enums.h"

class Projectile : public GameObject
{
public:



	Projectile();
	Projectile(GameObject* owner,WPTYPE wpType);
	virtual ~Projectile();

	//Projectile 만들기 시작
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;
	virtual bool CheckDead() override { return _isDead || (_elapsedTime >= _lifeTime); };
	virtual void OnCollision(GameObject* other) override;

	void SetDirection(Vec2<float> dir);

	//데미지는 한 방 컷이어서 일단 뺌
	//필요하면 추가하겠음 
	//void SetDamage(int damage);
	
	//owner의 의미는 이 Projectile을 누가 발사했는지를
	//기억하는 포인터이다
	void SetOwner(GameObject* owner);

	//Projectile 추가 변수
	GameObject* GetOwner() const { return _owner; }

	virtual void SaveToData(ObjectSpawnData& outData) override;
	virtual void LoadFromData(const ObjectSpawnData& spawnData) override;


	// setinfo 만들어야 함.
	void SetLethal(bool lethal) { _isLethal = lethal; };

	GameObject* GetOwner() { return _owner; }
	bool IsLethal() { return _isLethal; };
	ProjectileType GetProjType() { return _projType; }
	Vec2F GetDir() { return _dir; };

private:
	GameObject* _owner{ nullptr };         // 누가 발사/공격했는가 (자신이 맞지 않기 위해)
	ProjectileType _projType = ProjectileType::BULLET;   // 총알인지 근접인지

	//발사체 방향
	Vec2<float> _dir{};

	//발사체가 생성된 후 실제로 지난 시간
	float _elapsedTime = 0.0f;

	bool  _isLethal{ true };	// 살상용
	float _lifeTime{};            // 최대 생존 시간 (초)
	float _currentTimer{};        // 현재 생존 시간 계산용

	


};
