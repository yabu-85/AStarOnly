#pragma once
#include "EnemyBase.h"

class AstarMoveAction;

class AStarMan : public EnemyBase
{
    int hModel_;
    AstarMoveAction* pAstarMoveAction_;

public:
    AStarMan(GameObject* parent);
    ~AStarMan() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

