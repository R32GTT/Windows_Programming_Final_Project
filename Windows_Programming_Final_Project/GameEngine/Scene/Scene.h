#pragma once
#include "pch.h"

class GameObject;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC hdc) abstract;

	GameObject* GetGameObjectByID(unsigned int ID);

	std::vector<GameObject*> GetGameObjectLayer(GameObject* object) {
		return _objects[(int)object->layer];
	}



	void AddObject(GameObject* object);
	void RemoveObject(GameObject* object);

private:
	std::vector<GameObject*> _objects[(int)Layers::LAYER_COUNT];
};