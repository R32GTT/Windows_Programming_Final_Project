#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Managers.h"

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

	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Update();
}

void Scene::Render(HDC hdc, float alpha)
{
	Vec2<int> camPos = GET_SINGLE(SceneManager)->GetCameraPos();

	
	int camX = static_cast<int>(camPos.x) - WinSizeX/2;
	int camY = static_cast<int>(camPos.y) - WinSizeY/2;

	
	::SetWindowOrgEx(hdc, camX, camY, nullptr);

	
	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Render(hdc, alpha);
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
