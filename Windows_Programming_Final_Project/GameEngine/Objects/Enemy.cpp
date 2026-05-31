#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
{
	type = OBJECTTYPE::ENEMY;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
}

void Enemy::Update()
{
}

void Enemy::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Vec2<float> prevPos = GetRenderPos(alpha);
}

void Enemy::EmMove()
{
}

void Enemy::SetEnemyType(EnemyType etype)
{
	_enemyType = etype;
}

EnemyType Enemy::What_Enemy()
{
	if (_enemyType != EnemyType::NORMAL) {
		_enemyType = EnemyType::ARMORED;
		return (_enemyType);
	}
	else {
		return (_enemyType);
	}
}

EnemyType Enemy::GetEType()
{
	return _enemyType;
}
