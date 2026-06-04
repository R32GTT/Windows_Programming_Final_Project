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

	std::vector<GameObject*> GetAllObjects() {
		std::vector<GameObject*> allObjects;

		for (const std::vector<GameObject*>& layerObjects : _objects)
		{
			allObjects.insert(allObjects.end(), layerObjects.begin(), layerObjects.end());
		}

		return allObjects;
	}

	


	void AddObject(GameObject* object);
	void RemoveObject(GameObject* object);

	void BuildMapFromData(const MapData& mapData);
	void LinkObjectReferences(const MapData& mapData, const std::unordered_map<unsigned int, unsigned int>& idMap);


protected:
	Camera _cam;

private:
	GameObject* CreateObjectFromData(const ObjectSpawnData& data);

	std::vector<GameObject*> _objects[(int)Layers::LAYER_COUNT];
	std::unordered_map<unsigned int, GameObject*> _objectMap;
};