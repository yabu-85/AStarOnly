#pragma once
#include "Engine/GameObject.h"

class Player;
class EnemyBase;
class CollisionMap;

class Aim : public GameObject
{
    float mouseSensitivity;         //マウス感度
    float perspectiveDistance_;     //どのくらい後ろから映すか
    float heightDistance_;          //焦点の高さ
    float upMouselimit_;            //マウス移動の制限値（上下
    float donwMouselimit_;          //マウス移動の制限値（左右
    float mouseSpeed_;              //マウス移動スピード
    float defPerspectDistance_;     //デフォルトの視点の距離
    float heightRay_;               //Rayの値にプラスする量
    float numSupress_;              //マウス移動でOffsetの値を戻す量
    float maxCameraOffset_;         //cameraOffsetの最大距離
    float moveAimTime_;             //動く時の抑制の値
    float stopAimTime_;             //止まる時の抑制の値
    float targetRange_;             //ターゲットの有効範囲
    float fovRadian_;               //Targetの範囲
    float rotateRatio_;             //Target時の回転率
    bool aimMove_;                  //エイムを動かすかどうか
    bool isTarget_;                 //ターゲット状態か

    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPos_;            //カメラの場所目標
    XMFLOAT3 aimDirection_;         //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 plaPos_;               //プレイヤー位置
    XMFLOAT3 cameraOffset_;         //カメラの移動量

    Player* pPlayer_;
    EnemyBase* pEnemyBase_;
    CollisionMap* pCollisionMap_;

    void FacingTarget();
    void CalcCameraOffset(float _aimMove);
    void RayCastStage(XMFLOAT3 _start);

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //進行方向ベクトルのAim情報を取得
    XMFLOAT3 GetAimDirection() { return aimDirection_; }
    void SetAimMove(bool b) { aimMove_ = b; };
    XMFLOAT3 GetTargetPos();

    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

    //Targetの更新
    void SetTargetEnemy();
    bool IsTarget() { return isTarget_; };

    EnemyBase* GetTargetEnemy() { return pEnemyBase_; }

};