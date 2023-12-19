#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class Player;

class MoveAction : public BaseAction
{
    XMFLOAT3 targetPos_;//ターゲットの場所

protected:
    bool isInRange_;    //Targetについていたらtrue
    float moveSpeed_;   //移動スピード
    float moveRange_;   //移動終了判定の範囲

public:
    MoveAction(Character* obj, float speed, float range);
    virtual ~MoveAction() override {};
    virtual void Update() override;
    virtual void Initialize() override {};
    virtual void Terminate() override {};

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    bool IsInRange() { return isInRange_; }

    virtual void SetTarget(XMFLOAT3 target) { targetPos_ = target; }    //Astarの場合処理が違うためvirtual
};

class AstarMoveAction : public MoveAction {
    std::vector<XMFLOAT3> targetList_;
    bool isOutEndTarget_;

public:
    AstarMoveAction(Character* obj, float speed, float range);
    ~AstarMoveAction() {};

    void Update() override;
    void SetTarget(XMFLOAT3 target) override;
    std::vector<XMFLOAT3> GetTarget() { return targetList_; }
    void StopMove() { targetList_.clear(); }
    bool IsOutEndTarget() { return isOutEndTarget_; }

};