#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Enums.h"

class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual bool CheckDead();

	unsigned int GetID() const { return _id; };

	void SetPos(Vec2<float> POS) { pos = POS; };

	bool operator==(const GameObject& rhs) { return _id == rhs._id; };

	Layers GetLayer() { return layer; };


	Vec2<float> pos{};
	Vec2<float> facingDir{};
	Vec2<float> movingDir{};
	float speed{};

	static unsigned int _sNextId;

	unsigned int _id{};

	Layers layer = Layers::ACTORS;

	OBJECTTYPE type = OBJECTTYPE::NONE;

	//플레이어 상태를 위한 변수 추가
	PlayerState status = PlayerState::IDLE;
};