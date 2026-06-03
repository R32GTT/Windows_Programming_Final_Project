#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Camera.h"

struct MapData;


class GameObject;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) abstract;

	GameObject* GetGameObjectByID(unsigned int ID);

	std::vector<GameObject*> GetGameObjectLayer(GameObject* object) {
		return _objects[(int)object->GetLayer()];
	}

	std::vector<GameObject*> GetObjectsByLayer(Layers layer){
		return _objects[(int)layer];
	}



	void AddObject(GameObject* object);
	void RemoveObject(GameObject* object);

	void BuildMapFromData(const MapData& mapData);

protected:
	Camera _cam;

private:
	std::vector<GameObject*> _objects[(int)Layers::LAYER_COUNT];
	std::unordered_map<unsigned int, GameObject*> _objectMap;
};