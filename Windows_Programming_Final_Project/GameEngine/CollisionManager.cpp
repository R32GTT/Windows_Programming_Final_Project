#include "pch.h"
#include "Managers.h"
#include "Scene/Scene.h"
#include "CollisionManager.h"

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
}


void CollisionManager::ResolveWallCollision(GameObject* movingObj, GameObject* wall)
{
	Vec2<float> safePos = movingObj->GetPrevPos();
	movingObj->SetPos(safePos);
}

bool CollisionManager::CheckAABB(GameObject* a, GameObject* b)
{
	Vec2F diff = (a->GetPos() - b->GetPos()).Abs();

	Vec2F minDistance = a->GetHalfSize() + b->GetHalfSize();

	return (diff.x <= minDistance.x && diff.y <= minDistance.y);
}


void CollisionManager::Update()
{
	Scene* currentScene = GET_SINGLE(SceneManager)->GetCurrentScene();

	const auto& actors = currentScene->GetObjectsByLayer(Layers::ACTORS);
	const auto& walls = currentScene->GetObjectsByLayer(Layers::WALL);

	for (auto& actor : actors)
	{
		for (auto& wall : walls)
		{
			if (CheckAABB(actor, wall))
			{
				ResolveWallCollision(actor, wall);
			}
			else if (CheckOBB(actor, wall))
			{
				ResolveWallCollision(actor, wall);
			}
		}
	}


//OBB충돌 체크 구현시작
//혹시 문제가 있으면 삭제해도 괜찮음
bool CollisionManager::CheckOBB(GameObject* a, GameObject* b)
{

	return false;
}

