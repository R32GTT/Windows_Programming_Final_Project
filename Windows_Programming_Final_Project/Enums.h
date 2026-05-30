#pragma once

enum class SceneType
{
	NONE,
	DEVSCENE,
	EDITSCENE,
};


enum class OBJECTTYPE {
	NONE = 0,
	PLAYER,
	ENEMY,
	WEAPON,
	PROJECTILE,
	DECO,
	WALL,
	FLOOR,
	ENDPOINT,
	OBJECT_COUNT
};


enum class PlayerState
{
	IDLE,
	MOVE,
	EXECUTE,
	DEAD,
};

enum class EnemyState
{
	IDLE,
	PATROL,
	CHASE,
	ATTACK,

};

enum class EnemyType
{
	NORMAL = 0,
	// Dog,
	ARMORED
};

enum class Layers
{
	FLOOR,
	ITEM,
	ACTORS,
	UI,
	LAYER_COUNT
};