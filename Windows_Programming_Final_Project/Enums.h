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


enum class PlayerState
{
	IDLE,
	MOVE,
	EXECUTE,
	DEAD,
};

enum class Layers
{
	FLOOR,
	ITEM,
	ACTORS,
	UI,
	LAYER_COUNT
};