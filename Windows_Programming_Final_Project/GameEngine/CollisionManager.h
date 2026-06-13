#pragma once
#include "pch.h"

class GameObject;

class CollisionManager
{
public:
	DECLARE_SINGLE(CollisionManager);

	~CollisionManager();

	void Clear();
	void Init();
	void Update();

	void ResolveWallCollision(GameObject* movingObj, GameObject* wall);
	bool CheckLineOfSight(Vec2F start, Vec2F end);
private:
	bool CheckAABB(GameObject* a, GameObject* b);
	bool CheckOBB(GameObject* a, GameObject* b);
	bool CheckOBB_AABB(GameObject* obbObj, GameObject* aabbObj);


	
	void CheckActorWallCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& walls);
	void CheckProjectileCollision(const std::vector<GameObject*>& projectiles, const std::vector<GameObject*>& actors, const std::vector<GameObject*>& walls);
	//void CheckActorEndpointCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& endPoints);
	void CheckPlayerEnemyCollision(GameObject* player, const std::vector<GameObject*>& actors);
	void CheckActorWeaponCollision(const std::vector<GameObject*>& actors, const std::vector<GameObject*>& weapons);
	void CheckPlayerEndpointCollision(GameObject* player, const std::vector<GameObject*>& endPoints);

	std::vector<GameObject*> _characters;
	std::vector<GameObject*> _projectiles;
	std::vector<GameObject*> _endPoints;
	//std::vector<GameObject*> _weapons;


};