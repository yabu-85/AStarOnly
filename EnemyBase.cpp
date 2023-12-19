#include "EnemyBase.h"
#include "GameManager.h"
#include "Player.h"
#include "GameManager.h"

EnemyBase::EnemyBase(GameObject* parent)
	: Character(parent), type_(ENEMY_MAX), aimTargetPos_(0.0f)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Release()
{
}

void EnemyBase::ApplyDamage(int da)
{
	hp_ -= da;
	
}
