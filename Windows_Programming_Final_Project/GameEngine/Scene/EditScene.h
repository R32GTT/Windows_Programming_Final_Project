#pragma once
#include "Scene.h"

enum class EntityType
{
	NONE = 0,
	PlayerSpawn,
	Deco,
	Enemy,
	Weapon,
	Wall,
	Endpoint,
	ENTITY_COUNT
};

class EditScene : public Scene
{
public:
	using Super = Scene;
	EditScene();
	virtual ~EditScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	//맵 저장하고 불러오는 함수 선언
	void SaveMap(const std::wstring& fileName);
	void LoadMap(const std::wstring& fileName);

	
private:
	std::vector<std::pair<POINT, POINT>> _lines;

	bool _setOrigin = true;
	bool _playerSpawned = false;
	POINT _lastPos{ -9999, -9999 };
	POINT _prevMousePos{};

	POINT _wallStart{};
	bool _isDraggingWall = false;
	GameObject* _selectedObject = nullptr; 

	EntityType _currentEntity = EntityType::Wall;

	const int _gridSize = 16;
};