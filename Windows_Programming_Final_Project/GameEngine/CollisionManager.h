#pragma once
#include "pch.h"

class GameObject;

class CollisionManager
{
public:
	DECLARE_SINGLE(CollisionManager);

	~CollisionManager();

	void Init();
	void Update();

	void ResolveWallCollision(GameObject* movingObj, GameObject* wall);

private:
	bool CheckAABB(GameObject* a, GameObject* b);

	//OBB 충돌 체크 추가하기
	bool CheckOBB(GameObject* a, GameObject* b);

};