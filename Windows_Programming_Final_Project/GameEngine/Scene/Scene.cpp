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
	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			SAFE_DELETE(object);

	_objects->clear();

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

	_cam.TickComp();
}

void Scene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Vec2F camRenderPos = _cam.GetRenderPos(alpha);

	GET_SINGLE(SceneManager)->SetCameraPos(camRenderPos);

	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Render(renderTarget, alpha);
}

GameObject* Scene::GetGameObjectByID(unsigned int ID)
{
	auto iter = _objectMap.find(ID);

	if (iter != _objectMap.end())
		return iter->second;

	return nullptr;
}

void Scene::AddObject(GameObject* object)
{
	if (object == nullptr)
		return;

	_objects[(int)object->GetLayer()].push_back(object);
	_objectMap[object->GetID()] = object;
}

void Scene::RemoveObject(GameObject* object)
{
	if (object == nullptr)
		return;

	std::vector<GameObject*>& v = _objects[(int)object->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), object), v.end());
	_objectMap.erase(object->GetID());
}

void Scene::BuildMapFromData(const MapData& mapData)
{
	std::unordered_map<unsigned int, unsigned int> idMap;

	for (const ObjectSpawnData& spawnData : mapData.objects)
	{
		GameObject* newObj = nullptr;

		switch (spawnData.objType)
		{
		case OBJECTTYPE::NONE:
			break;
		case OBJECTTYPE::PLAYER:
			newObj = new Player();
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

		if (newObj != nullptr)
		{
			newObj->LoadFromData(spawnData);
		}
	}

}
