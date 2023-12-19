#pragma once
#include "Character.h"
#include "EnemyManager.h"

class EnemyBase : public Character
{
protected:
    float aimTargetPos_;        //Aim��Target���鎞�̏ꏊ
    ENEMY_TYPE type_;

public:
    EnemyBase(GameObject* parent);
    virtual ~EnemyBase();
    virtual void Initialize() override = 0;
    virtual void Update() override = 0;
    virtual void Draw() override = 0;
    virtual void Release();

    virtual void ApplyDamage(int da) override;
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    float GetAimTargetPos() { return aimTargetPos_; }
};

