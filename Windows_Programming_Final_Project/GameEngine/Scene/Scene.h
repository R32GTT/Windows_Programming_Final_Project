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

	void AddObject(GameObject* object);
	void RemoveObject(GameObject* object);

private:
	std::vector<GameObject*> _objects[(int)Layers::LAYER_COUNT];
};