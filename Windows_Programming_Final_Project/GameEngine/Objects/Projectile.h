#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class Projectile : public GameObject
{
public:
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

private:

	//private도 추가함
	Vec2<float> _dir{};
	int _damage = 10;

	float _lifeTime = 2.0f;
	float _elapsedTime = 0.0f;

	GameObject* _owner = nullptr;

};