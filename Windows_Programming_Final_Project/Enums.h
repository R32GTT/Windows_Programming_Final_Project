#pragma once

enum class SceneType
{
	NONE,
	DEVSCENE,
	EDITSCENE,
	PLAYSCENE
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
	ATTACK,
	THROW,
	EXECUTE,
	DEAD,
};

enum class EnemyState
{
	IDLE,
	PATROL,
	CHASE,
	ATTACK,
	UNCONSCIOUS,
	DEAD
};

enum class EnemyType
{
	NORMAL = 0,
	// Dog,
	ARMORED,
	ETYPE_COUNT
};

enum class Layers
{
	FLOOR,
	WALL,
	DECO,
	ITEM,
	ACTORS,
	UI,
	LAYER_COUNT
};

enum class WPTYPE {
	NONE = 0,
	FIST,
	//KNIFE, TODO Maybe someday...
	CROWBAR,
	//AXE,
	//PISTOL,
	//SHOTGUN,
	RIFLE,
	RANDOM_ANY,
	TOTAL_COUNT
};

//HEEEEEEEEELLLLLLLLPPPPPPPPPPPPP
enum class AnimType
{
	IDLE,
	MOVE,
	ATTACK_FIST,
	IDLE_CROWBAR,
	ATTACK_CROWBAR,
	IDLE_GUN,
	ATTACK_GUN,
	EXECUTE,
	UNCONSCIOUS,
	DEAD,
	TOTAL_COUNT
};


enum class ProjectileType
{
	BULLET,
	MELEE
};