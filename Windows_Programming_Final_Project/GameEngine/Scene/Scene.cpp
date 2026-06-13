#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "../Objects/Objects.h"
#include "Managers.h"
#include "../Utility/Vec2.h"
#include "../LevelData/LevelData.h"	

Scene::Scene()
{

}

Scene::~Scene()
{
	for (int i = 0; i < (int)Layers::LAYER_COUNT; ++i)
	{
		for (GameObject* object : _objects[i])
		{
			SAFE_DELETE(object);
		}
		_objects[i].clear(); // 각 레이어별로 vector를 확실히 비워줌
	}
	_player = nullptr;
	_objectMap.clear();
	_spawnQueue.clear();
}

void Scene::Init()
{
	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Init();

}

void Scene::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	
	_cam.SavePrevPos();

	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Update();

	GET_SINGLE(CollisionManager)->Update();

	PurgeDeadObjects();
	FlushSpawnQueue();
	_cam.TickComp();
}

void Scene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Vec2F camRenderPos = _cam.GetRenderPos(alpha);

	GET_SINGLE(SceneManager)->SetCameraPos(camRenderPos);

	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Render(renderTarget, alpha);
	if (_player)
		_player->Render(renderTarget, alpha);
}

void Scene::Clear()
{
	for (int i = 0; i < (int)Layers::LAYER_COUNT; ++i)
	{
		for (GameObject* object : _objects[i])
		{
			SAFE_DELETE(object);
		}
		_objects[i].clear(); // 각 레이어별로 vector를 확실히 비워줌
	}
	_player = nullptr;
	_objectMap.clear();
	_spawnQueue.clear();
}

void Scene::PurgeDeadObjects()
{
	for (int i = 0; i < (int)Layers::LAYER_COUNT; ++i)
	{
		auto& layerVec = _objects[i];

		// 조건(CheckDead)에 맞는 오브젝트들을 뒤로 밀고 지워버리는 가비지 컬렉션 메커니즘
		auto it = std::remove_if(layerVec.begin(), layerVec.end(), [this](GameObject* obj) {
			if (obj && obj->CheckDead())
			{
				_objectMap.erase(obj->GetID()); 
				SAFE_DELETE(obj);               
				return true;                    
			}
			return false;
			});

		layerVec.erase(it, layerVec.end());
	}
}

GameObject* Scene::GetGameObjectByID(unsigned int ID)
{
	auto iter = _objectMap.find(ID);

	if (iter != _objectMap.end())
		return iter->second;

	return nullptr;
}

bool Scene::CheckSceneCleared()
{
	std::vector<GameObject*> enemies = GetObjectsByLayer(Layers::ACTORS);

	for (GameObject* obj : enemies)
	{
		if (obj->GetObjectType() == OBJECTTYPE::ENEMY)
		{
			if (!obj->IsKilled())
				return false;
		}
	}

	return true;
}

void Scene::AddObject(GameObject* object)
{
	if (object == nullptr)
		return;
	_spawnQueue.push_back(object);
}

void Scene::FlushSpawnQueue()
{
	if (_spawnQueue.empty())
		return;

	for (GameObject* newObj : _spawnQueue)
	{
		_objects[(int)newObj->GetLayer()].push_back(newObj);
		_objectMap[newObj->GetID()] = newObj;
	}

	_spawnQueue.clear();
}

void Scene::RemoveObject(GameObject* object)
{
	if (object == nullptr)
		return;

	std::vector<GameObject*>& v = _objects[(int)object->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), object), v.end());
	_objectMap.erase(object->GetID());

	SAFE_DELETE(object);
}

void Scene::BuildMapFromData(const MapData& mapData)
{
	std::unordered_map<unsigned int, unsigned int> idMap;

	for (const ObjectSpawnData& spawnData : mapData.objects)
	{
		GameObject* newObj = CreateObjectFromData(spawnData);

		if (newObj != nullptr)
		{
			newObj->LoadFromData(spawnData);
			AddObject(newObj);
			if (newObj->GetObjectType() == OBJECTTYPE::PLAYER)
				SetPlayer(newObj);
			idMap[spawnData.fileID] = newObj->GetID();
		}
	}
	FlushSpawnQueue();
}

void Scene::LinkObjectReferences(const MapData& mapData, const std::unordered_map<unsigned int, unsigned int>& idMap)
{
	/* for (const ObjectSpawnData& spawnData : mapData.objects)
	{
		if (spawnData.targetId != 0)
		{
			unsigned int realOwnerId = idMap.at(spawnData.id);
			unsigned int realTargetId = idMap.at(spawnData.targetId);

			GameObject* owner = GetGameObjectByID(realOwnerId);
			GameObject* target = GetGameObjectByID(realTargetId);
			// owner->SetTarget(target);
		}
	}
	*/
}

//무기 스폰 함수
void Scene::SpawnWeapon(Vec2<float> pos) {

	Weapon* weapon = new Weapon();

	weapon->SetPos(pos);
	weapon->Init();

	AddObject(weapon);
}


GameObject* Scene::CreateObjectFromData(const ObjectSpawnData& data)
{
	GameObject* newObj = nullptr;
	switch (data.objType)
	{
	case OBJECTTYPE::NONE:
		break;
	case OBJECTTYPE::PLAYER:
		newObj = new Player();
		_player = newObj;
		break;
	case OBJECTTYPE::ENEMY:
		newObj = new Enemy();
		break;
	case OBJECTTYPE::WEAPON:
		newObj = new Weapon();
		break;
	case OBJECTTYPE::PROJECTILE:
		newObj = new Projectile();
		break;
	case OBJECTTYPE::DECO:
		newObj = new DECO();
		break;
	case OBJECTTYPE::WALL:
		newObj = new Wall();
		break;
	case OBJECTTYPE::FLOOR:
		newObj = new Floor();
		break;
	case OBJECTTYPE::ENDPOINT:
		newObj = new EndPoint();
		break;
	case OBJECTTYPE::OBJECT_COUNT:
		break;
	default:
		break;
	}
	return newObj;
}
