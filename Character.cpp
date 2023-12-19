#include "Character.h"

Character::Character(GameObject* parent) 
	: GameObject(parent), hp_(0), maxHp_(0), movement_(XMVectorZero())
{
}

void Character::ApplyDamage(int da)
{
	hp_ -= da;
}