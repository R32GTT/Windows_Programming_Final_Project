#pragma once
#include "Scene.h"

enum class EntityType
{
	NONE = 0,
	PlayerSpawn,
	Enemy,
	Weapon,
	Wall,
	Endpoint,
	ENTITY_COUNT
};

class EditScene : public Scene
{
public:
	EditScene();
	virtual ~EditScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc, float alpha) override;

	////맵 저장하고 불러오는 함수 선언
	//void SaveMap(const char* fileName);
	//void LoadMap(const char* fileName);
	// wstring 존재하는데 const char 쓸 필요가 없음


private:
	std::vector<std::pair<POINT, POINT>> _lines;

	bool _setOrigin = true;
	POINT _lastPos{};
	POINT _prevMousePos{};

	EntityType _currentEntity = EntityType::Wall;

	const int _gridSize = 16;
};