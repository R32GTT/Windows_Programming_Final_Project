#include "pch.h"
#include "Managers.h"
#include "Scene/Scene.h"
#include "CollisionManager.h"
#include "Objects/Projectile.h"
#include "Objects/Enemy.h"

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
}

void CollisionManager::ResolveWallCollision(GameObject* movingObj, GameObject* wall)
{
	Vec2F objPos = movingObj->GetPos();
	Vec2F diff = objPos - wall->GetPos();
	Vec2F minDistance = movingObj->GetHalfSize() + wall->GetHalfSize();
	Vec2F overlap = minDistance - diff.Abs();

	if (overlap.x < overlap.y)
		objPos.x += (diff.x > 0) ? overlap.x : -overlap.x;
	else
		objPos.y += (diff.y > 0) ? overlap.y : -overlap.y;

	movingObj->SetPos(objPos);
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
	if (currentScene == nullptr) return;

	_characters.clear();
	_projectiles.clear();
	_endPoints.clear();

	const auto& allActors = currentScene->GetObjectsByLayer(Layers::ACTORS);
	const auto& walls = currentScene->GetObjectsByLayer(Layers::WALL);

	// 2. Scene이 관리하는 안전한 객체들만 분류
	for (auto& obj : allActors)
	{
		if (obj == nullptr || obj->CheckDead()) continue;

		OBJECTTYPE type = obj->GetObjectType();

		if (type == OBJECTTYPE::PLAYER || type == OBJECTTYPE::ENEMY)
			_characters.push_back(obj);
		else if (type == OBJECTTYPE::PROJECTILE)
			_projectiles.push_back(obj);
		else if (type == OBJECTTYPE::ENDPOINT)
			_endPoints.push_back(obj);
	}
	GameObject* player = currentScene->GetPlayer();
	

	CheckActorWallCollision(_characters, walls);
	CheckProjectileCollision(_projectiles, _characters, walls);
	CheckActorEndpointCollision(_characters, _endPoints);
	if (player != nullptr && !player->IsKilled())
		CheckPlayerEnemyCollision(player, _characters);
	CheckActorWeaponCollision(_characters, currentScene->GetObjectsByLayer(Layers::ITEM));

}

void CollisionManager::CheckActorWallCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& walls)
{
	for (auto* actor : actors)
	{
		if (actor == nullptr || actor->CheckDead()) continue;

		for (auto* wall : walls)
		{
			if (wall == nullptr) continue;
			if (actor->IsKilled()) continue;

			float rA = actor->GetHalfSize().x + actor->GetHalfSize().y;
			float rB = wall->GetHalfSize().x + wall->GetHalfSize().y;
			float maxDist = rA + rB;

			Vec2F diff = actor->GetPos() - wall->GetPos();
			if (diff.LengthSq() > maxDist * maxDist) continue;

			if (CheckOBB_AABB(actor, wall))
			{
				ResolveWallCollision(actor, wall);
				actor->OnCollision(wall);
				wall->OnCollision(actor);
			}
		}
	}
}

void CollisionManager::CheckProjectileCollision(const std::vector<GameObject*>& projectiles, const std::vector<GameObject*>& actors, const std::vector<GameObject*>& walls)
{
	for (auto* roj : projectiles)
	{
		if (roj == nullptr || roj->CheckDead()) continue;

		Projectile* proj = static_cast<Projectile*>(roj);
		GameObject* owner = proj->GetOwner(); 

		for (auto* wall : walls)
		{
			if (wall == nullptr) continue;

			Vec2F diff = proj->GetPos() - wall->GetPos();
			float maxDist = (proj->GetHalfSize().x + proj->GetHalfSize().y) + (wall->GetHalfSize().x + wall->GetHalfSize().y);
			if (diff.LengthSq() > maxDist * maxDist) continue;

			if (CheckOBB_AABB(proj, wall))
			{
				proj->OnCollision(wall);
				wall->OnCollision(proj);
				break;
			}
		}

		if (proj->CheckDead()) continue;

		for (auto* actor : actors)
		{
			if (actor == nullptr || actor->CheckDead()) continue;

			if (actor == owner) continue;
			if (actor->IsKilled()) continue;

			Vec2F diff = proj->GetPos() - actor->GetPos();
			float maxDist = (proj->GetHalfSize().x + proj->GetHalfSize().y) + (actor->GetHalfSize().x + actor->GetHalfSize().y);
			if (diff.LengthSq() > maxDist * maxDist) continue;

			if (CheckOBB(proj, actor))
			{
				proj->OnCollision(actor);
				actor->OnCollision(proj);
				if (proj->CheckDead()) break; 
			}
		}
	}
}

void CollisionManager::CheckActorEndpointCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& endPoints)
{
	for (auto* actor : actors)
	{
		if (actor == nullptr || actor->CheckDead()) continue;
		if (actor->GetObjectType() != OBJECTTYPE::PLAYER) continue;

		for (auto* ep : endPoints)
		{
			if (ep == nullptr) continue;

			Vec2F diff = actor->GetPos() - ep->GetPos();
			float maxDist = (actor->GetHalfSize().x + actor->GetHalfSize().y) + (ep->GetHalfSize().x + ep->GetHalfSize().y);
			if (diff.LengthSq() > maxDist * maxDist) continue;

			if (CheckOBB_AABB(actor, ep))
			{
				//actor->OnCollision(ep);
				ep->OnCollision(actor);
				return;
			}
		}
	}
}


void CollisionManager::CheckPlayerEnemyCollision(GameObject* player, const std::vector<GameObject*>& actors)
{
	for (auto* obj : actors)
	{
		if (obj == nullptr || obj->CheckDead()) continue;
		if (obj->GetObjectType() != OBJECTTYPE::ENEMY) continue;


		Enemy* enemy = static_cast<Enemy*>(obj);
		if (enemy->IsKilled()) continue; 

		Vec2F diff = player->GetPos() - enemy->GetPos();
		float maxDist = (player->GetHalfSize().x + player->GetHalfSize().y) + (enemy->GetHalfSize().x + enemy->GetHalfSize().y);
		if (diff.LengthSq() > maxDist * maxDist) continue;

		if (CheckOBB(player, enemy))
		{
			player->OnCollision(enemy);
			enemy->OnCollision(player);
		}
	}
}

void CollisionManager::CheckActorWeaponCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& weapons)
{
	for (auto* actor : actors)
	{
		if (actor == nullptr || actor->CheckDead()) continue;
		OBJECTTYPE objt = actor->GetObjectType();
		if (objt != OBJECTTYPE::PLAYER && objt != OBJECTTYPE::ENEMY) continue;

		for (auto* wep : weapons)
		{
			if (wep == nullptr) continue;
			if (wep->CheckDead()) continue;

			Vec2F diff = actor->GetPos() - wep->GetPos();
			float maxDist = (actor->GetHalfSize().x + actor->GetHalfSize().y) + (wep->GetHalfSize().x + wep->GetHalfSize().y);
			if (diff.LengthSq() > maxDist * maxDist) continue;

			if (CheckOBB(actor, wep))
			{
				actor->OnCollision(wep);
				//wep->OnCollision(actor);
				break;
			}
		}
	}
}

//OBB충돌 체크 구현시작
//혹시 문제가 있으면 삭제해도 괜찮음
bool CollisionManager::CheckOBB(GameObject* a, GameObject* b)
{
	Vec2F aHalf = a->GetHalfSize();
	Vec2F bHalf = b->GetHalfSize();

	Vec2F aFace = a->GetFacningDir();
	if (aFace.LengthSq() == 0.f) aFace = { 1.0f, 0.0f };
	Vec2F aUp = aFace.Normalized();
	Vec2F aRight = { -aUp.y, aUp.x };

	Vec2F bFace = b->GetFacningDir();
	if (bFace.LengthSq() == 0.f) bFace = { 1.0f, 0.0f };
	Vec2F bUp = bFace.Normalized();
	Vec2F bRight = { -bUp.y, bUp.x };

	Vec2F centerDiff = b->GetPos() - a->GetPos();
	Vec2F axes[4] = { aRight, aUp, bRight, bUp };

	for (int i = 0; i < 4; i++)
	{
		Vec2F axis = axes[i];

		float distance = std::abs(centerDiff.Dot(axis));

		float aRadius = aHalf.x * std::abs(aRight.Dot(axis)) + aHalf.y * std::abs(aUp.Dot(axis));
		float bRadius = bHalf.x * std::abs(bRight.Dot(axis)) + bHalf.y * std::abs(bUp.Dot(axis));

	
		if (distance >= aRadius + bRadius) return false;
	}
	return true;
}

bool CollisionManager::CheckOBB_AABB(GameObject* obbObj, GameObject* aabbObj)
{
	Vec2F oHalf = obbObj->GetHalfSize();
	Vec2F aHalf = aabbObj->GetHalfSize();

	Vec2F oFace = obbObj->GetFacningDir();
	if (oFace.LengthSq() == 0.f) oFace = { 0.0f, -1.0f };
	Vec2F oUp = oFace.Normalized();
	Vec2F oRight = { -oUp.y, oUp.x };

	Vec2F centerDiff = aabbObj->GetPos() - obbObj->GetPos();

	Vec2F axes[4] = { oRight, oUp, {1.0f, 0.0f}, {0.0f, 1.0f} };

	for (int i = 0; i < 4; i++)
	{
		Vec2F axis = axes[i];


		float distance = std::abs(centerDiff.Dot(axis));

		float oRadius = oHalf.x * std::abs(oRight.Dot(axis)) + oHalf.y * std::abs(oUp.Dot(axis));
		float aRadius = aHalf.x * std::abs(axis.x) + aHalf.y * std::abs(axis.y);

		if (distance >= oRadius + aRadius) return false;
	}
	return true;
}

