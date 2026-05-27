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

void Scene::Render(HDC hdc)
{
	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Render(hdc);
}

GameObject* Scene::GetGameObjectByID(unsigned int ID)
{
	return nullptr;
}

void Scene::AddObject(GameObject* object)
{
	if (object == nullptr)
		return;

	_objects[(int)object->GetLayer()].push_back(object);
}

void Scene::RemoveObject(GameObject* object)
{
	if (object == nullptr)
		return;

	std::vector<GameObject*>& v = _objects[(int)object->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), object), v.end());
}
