#include "pch.h"
#include "Managers.h"
#include "Scene/Scene.h"
#include "CollisionManager.h"
#include "Objects/Projectile.h"
#include "Objects/Enemy.h"

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Clear()
{
	_characters.clear();
	_projectiles.clear();
	_endPoints.clear();
}

void CollisionManager::Init()
{
}

void CollisionManager::ResolveWallCollision(GameObject* movingObj, GameObject* wall)
{
	Vec2F objPos = movingObj->GetPos();
	Vec2F diff = objPos - wall->GetPos();

	// 1. 캐릭터가 바라보는 방향(회전)을 바탕으로 로컬 축 구하기
	Vec2F oFace = movingObj->GetFacningDir();
	if (oFace.LengthSq() == 0.f) oFace = { 0.0f, -1.0f };
	Vec2F oUp = oFace.Normalized();
	Vec2F oRight = { -oUp.y, oUp.x };

	Vec2F oHalf = movingObj->GetHalfSize();

	// 2. [핵심] 캐릭터가 회전했을 때, 월드 공간(X, Y축)에서 차지하는 '실제 반경' 계산
	Vec2F effectiveHalfSize;
	effectiveHalfSize.x = oHalf.x * std::abs(oRight.x) + oHalf.y * std::abs(oUp.x);
	effectiveHalfSize.y = oHalf.x * std::abs(oRight.y) + oHalf.y * std::abs(oUp.y);

	// 3. 기존의 GetHalfSize() 대신, 회전이 고려된 effectiveHalfSize를 사용하여 겹친 깊이 계산
	Vec2F minDistance = effectiveHalfSize + wall->GetHalfSize();
	Vec2F overlap = minDistance - diff.Abs();

	// 컴퓨터 소수점 오차로 인해 다음 프레임에 또 끼는 것을 방지할 여유 공간(Epsilon)
	float epsilon = 0.1f;

	// 겹친 부분이 있을 때만 밀어내기 처리
	if (overlap.x > 0 && overlap.y > 0)
	{
		// X, Y 중 덜 겹친 방향(가장 빠르게 탈출할 수 있는 방향)으로 밀어내기 -> 자연스러운 벽 타기 완성
		if (overlap.x < overlap.y)
			objPos.x += (diff.x > 0) ? (overlap.x + epsilon) : -(overlap.x + epsilon);
		else
			objPos.y += (diff.y > 0) ? (overlap.y + epsilon) : -(overlap.y + epsilon);
	}

	// 최종 위치 갱신
	movingObj->SetPos(objPos);
}

bool CollisionManager::CheckLineOfSight(Vec2F start, Vec2F end)
{
	Scene* currentScene = GET_SINGLE(SceneManager)->GetCurrentScene();
	if (currentScene == nullptr) return false;

	const auto& walls = currentScene->GetObjectsByLayer(Layers::WALL);

	Vec2F travelVec = end - start;
	float travelDist = travelVec.Length();

	// 10픽셀 단위로 선분을 잘라서 검사 (맵의 가장 얇은 벽 두께보다 작게 설정하면 절대 뚫리지 않음)
	float stepSize = 10.0f;
	int steps = std::max(1, static_cast<int>(travelDist / stepSize));

	for (int i = 1; i <= steps; ++i)
	{
		float t = static_cast<float>(i) / steps;
		Vec2F checkPoint = start + travelVec * t;

		// 쪼개진 점(checkPoint)이 벽 내부에 들어갔는지 검사
		for (auto* wall : walls)
		{
			if (wall == nullptr || wall->CheckDead()) continue;

			// 점과 AABB(벽)의 충돌 검사 (가장 가벼운 연산)
			Vec2F diff = (checkPoint - wall->GetPos()).Abs();

			if (diff.x <= wall->GetHalfSize().x && diff.y <= wall->GetHalfSize().y)
			{
				return false; // 시야가 벽에 가로막힘!
			}
		}
	}

	return true; // 모든 벽을 통과했다면 시야가 확보됨!
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
	if (player != nullptr && !player->IsKilled())
	{
		CheckPlayerEnemyCollision(player, _characters);
		CheckPlayerEndpointCollision(player, _endPoints); 
	}
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

		// 1. 이번 프레임의 이동 궤적 정보 계산
		Vec2F startPos = proj->GetPrevPos();
		Vec2F endPos = proj->GetPos();
		Vec2F travelVec = endPos - startPos;
		float travelDist = travelVec.Length();

		// [핵심 변경점] 총알의 가장 얇은 부분을 기준으로 stepSize 자동 설정
		// GetHalfSize()는 절반 크기이므로, 이 값을 stepSize로 쓰면 절대 건너뛰지 않는 안전한 간격이 됩니다.
		float minHalfSize = std::min(proj->GetHalfSize().x, proj->GetHalfSize().y);
		float stepSize = (minHalfSize > 0.1f) ? minHalfSize : 2.0f; // 0이 되는 것을 방지하는 최소한의 방어 코드

		int steps = std::max(1, static_cast<int>(travelDist / stepSize));

		bool isProjectileDestroyed = false;

		// 2. 궤적을 따라 잘게 쪼개서 한 단계씩 전진하며 검사
		for (int i = 1; i <= steps; ++i)
		{
			float t = static_cast<float>(i) / steps;
			Vec2F currentPos = startPos + travelVec * t;

			proj->SetPos(currentPos);


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


					if (proj->CheckDead())
					{
						isProjectileDestroyed = true;
						break;
					}
				}
			}

			if (isProjectileDestroyed || proj->CheckDead()) break;

			// [B] 액터(적/플레이어) 충돌 검사
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
					if (proj->CheckDead())
					{
						isProjectileDestroyed = true;
						break;
					}
				}
			}

			if (isProjectileDestroyed || proj->CheckDead()) break;
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

void CollisionManager::CheckPlayerEndpointCollision(GameObject* player, const std::vector<GameObject*>& endPoints)
{
	// Update에서 체크하긴 하지만 안전을 위해 한 번 더 검증
	if (player == nullptr || player->CheckDead()) return;

	for (auto* ep : endPoints)
	{
		if (ep == nullptr) continue;

		Vec2F diff = player->GetPos() - ep->GetPos();
		float maxDist = (player->GetHalfSize().x + player->GetHalfSize().y) + (ep->GetHalfSize().x + ep->GetHalfSize().y);
		if (diff.LengthSq() > maxDist * maxDist) continue;

		if (CheckOBB_AABB(player, ep))
		{
			ep->OnCollision(player);
			return; 
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

