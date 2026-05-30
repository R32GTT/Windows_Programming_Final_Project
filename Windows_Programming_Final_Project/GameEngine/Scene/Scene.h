#pragma once
#include "pch.h"
#include "GameObject.h"

class GameObject;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC hdc, float alpha) abstract;

	GameObject* GetGameObjectByID(unsigned int ID);

	std::vector<GameObject*> GetGameObjectLayer(GameObject* object) {
		return _objects[(int)object->GetLayer()];
	}



	void AddObject(GameObject* object);
	void RemoveObject(GameObject* object);

private:
	std::vector<GameObject*> _objects[(int)Layers::LAYER_COUNT];
	std::unordered_map<unsigned int, GameObject*> _objectMap;
};