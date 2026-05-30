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
	virtual void Render(HDC hdc) override;

	//맵 저장하고 불러오는 함수 선언
	//fstream 사용으로 변경
	//fstream은 잠시 보류
	void SaveMap(const std::string& fileName);
	void LoadMap(const std::string& fileName);


private:
	std::vector<std::pair<POINT, POINT>> _lines;

	bool _setOrigin = true;
	POINT _lastPos{};
	POINT _prevMousePos{};

	EntityType _currentEntity = EntityType::Wall;

	const int _gridSize = 16;
};