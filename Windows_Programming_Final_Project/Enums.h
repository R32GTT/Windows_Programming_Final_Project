#pragma once

enum class SceneType
{
	NONE,
	DEVSCENE,
	EDITSCENE,
};


enum class OBJECTTYPE {
	NONE = 0,
	PLAYER = 1,
	ENEMY = 2,
	WEAPON = 3,
	PROJECTILE,
	DECO,
	WALL,
	FLOOR
};

//ALIVE 하나 추가 됨
enum class PlayerState
{
	IDLE,
	MOVE,
	EXECUTE,
	DEAD,
	ALIVE
};

enum class Layers
{
	FLOOR,
	ITEM,
	ACTORS,
	UI,
	LAYER_COUNT
};