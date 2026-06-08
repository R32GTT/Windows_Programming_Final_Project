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

	//wallCollision과 ProjectileCollision추가
	CheckActorWallCollision(actors, walls);
	//CheckProjectileCollision(actors, walls);


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
}

void CollisionManager::CheckActorWallCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& walls)
{


}

//void CollisionManager::CheckProjectileCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject>& walls)


//OBB충돌 체크 구현시작
//혹시 문제가 있으면 삭제해도 괜찮음
bool CollisionManager::CheckOBB(GameObject* a, GameObject* b)
{

	//Half Center
	//Right Up
	Vec2F aCenter = a->GetPos();
	Vec2F bCenter = b->GetPos();

	Vec2F aHalf = a->GetHalfSize();
	Vec2F bHalf = b->GetHalfSize();

	//a의 회전 축
	//Normailize를 해야 함
	Vec2F aRight = a->GetFacningDir().Normalized();
	Vec2F aUp = Vec2F(-aRight.y, aRight.x);

	//b의 회전 축
	Vec2F bRight = b->GetFacningDir().Normalized();
	Vec2F bUp = Vec2F(-bRight.y, bRight.x);

	Vec2F ceterDiff = bCenter - aCenter;


	Vec2F axes[4] = {

		aRight,
		aUp,
		bRight,
		bUp


	};

	//투영 부분
	for (int i = 0; i < 4; i++)
	{
		Vec2F axis = axes[i];

		//두 중심 사이 거리를 현재 축에 투영
		float distance = fabs(ceterDiff.Dot(axis));

		//a의 반지름을 현재 축에 투영
		float aRadius = aHalf.x * fabs(aRight.Dot(axis)) + aHalf.y * fabs(aUp.Dot(axis));

		//b의 반지름을 현재 축에 투영
		float bRadius = bHalf.x * fabs(bRight.Dot(axis)) + bHalf.y * fabs(bUp.Dot(axis));

		//이 축에서 만약 떨어져 있다면 충돌이 아니다
		if (distance > aRadius + bRadius)
		{
			return false;
		}


	}


	//그런데 모든 축에서 겹치면 충돌이라고 한다
	return true;
}

