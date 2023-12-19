#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "PlayerCommand.h"
#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //移動スピードの加減の値
    const float stopGradually = 0.25f;      //移動スピードの加減の値止まるとき
    const float moveGradually = 0.15f;      //移動スピードの加減の値移動時
    const float maxMoveSpeed = 1.0f;        //最大移動スピード
    const float avoRotateRatio = 0.92f;     //回避時のRotateRatio

    int testModel = -1;
    bool isCollider = true; //当たり判定するかどうか、浮けるようにするための

    Text* pText = new Text;
    XMFLOAT3 rotateMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
    SphereCollider* collid = nullptr;
    XMFLOAT3 prePos = XMFLOAT3();
}

Player::Player(GameObject* parent)
    : Character(parent), hModel_{-1, -1}, pAim_(nullptr), playerMovement_{0,0,0}, pCommand_(nullptr), moveSpeed_(0.0f), rotateRatio_(0.0f)
{
    objectName_ = "Player";
}

Player::~Player()
{
    delete pCommand_;
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_[0] = Model::Load("Model/FiterTestUp.fbx");
    assert(hModel_[0] >= 0);

    testModel = Model::Load("Model/SphereCollider.fbx");
    assert(testModel >= 0);

    hModel_[1] = Model::Load("Model/FiterTestDown.fbx");
    assert(hModel_[1] >= 0);
    transform_.rotate_.y += 180.0f;

    pAim_ = Instantiate<Aim>(this);
    pCommand_ = new PlayerCommand();

    maxHp_ = 100;
    hp_ = maxHp_;
    moveSpeed_ = 0.15f;
    rotateRatio_ = 0.2f;

    collid = new SphereCollider(XMFLOAT3(0.0f, 2.3f, 0.0f), 0.5f);
    AddCollider(collid);

    pText->Initialize();
}

void Player::Update()
{
    pCommand_->Update();

    //エイムターゲット
    if (pCommand_->CmdTarget()) pAim_->SetTargetEnemy();
    
    //デバッグ用
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
    if (Input::IsKey(DIK_H)) ApplyDamage(1);

    XMVECTOR vec = XMLoadFloat3(&transform_.position_) - XMLoadFloat3(&prePos);
    vec *= 10.0f;
    if (1.0f <= XMVectorGetX(XMVector3Length(vec))) {
        vec = XMVector3Normalize(vec);
    }
    movement_ = vec;

    CalcMove();
    Move();
    Rotate();

}

void Player::Draw()
{
    Model::SetTransform(hModel_[0], transform_);
    Model::Draw(hModel_[0]);
    Model::SetTransform(hModel_[1], transform_);
    Model::Draw(hModel_[1]);

    Transform test = transform_;
    test.position_.y += 0.75f;
    test.scale_.x = 0.2f;
    test.scale_.y = 0.2f;
    test.scale_.z = 0.2f;
    Model::SetTransform(testModel, test);
    Model::Draw(testModel, 4);

    CollisionDraw();

    //デバッグ用
    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);
    
    pText->Draw(1100, 30, money_);
    pText->Draw(1100, 70, hp_);
    pText->Draw(1150, 70, " / ");
    pText->Draw(1200, 70, maxHp_);
    
}

void Player::Release()
{
}

void Player::Rotate()
{
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(rotateMove.x, rotateMove.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);

    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void Player::Rotate(float ratio)
{
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(rotateMove.x, rotateMove.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);

    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * ratio);
}

XMFLOAT3 Player::GetInputMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    if (pCommand_->CmdWalk()) {
        gradually = moveGradually;

        XMFLOAT3 aimDirection = pAim_->GetAimDirection();
        if (pCommand_->CmdUp()) {
            fMove.x += aimDirection.x;
            fMove.z += aimDirection.z;
        }
        if (pCommand_->CmdLeft()) {
            fMove.x -= aimDirection.z;
            fMove.z += aimDirection.x;
        }
        if (pCommand_->CmdDown()) {
            fMove.x -= aimDirection.x;
            fMove.z -= aimDirection.z;
        }
        if (pCommand_->CmdRight()) {
            fMove.x += aimDirection.z;
            fMove.z -= aimDirection.x;
        }
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);

    return fMove;
}

void Player::FrontMove(float f)
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    vMove = XMVector3Normalize(vMove + XMLoadFloat3(&playerMovement_));
    XMFLOAT3 move{};
    XMStoreFloat3(&move, vMove);

    transform_.position_.x += ((move.x * moveSpeed_) * f);
    transform_.position_.z += ((move.z * moveSpeed_) * f);
}

void Player::Move(float f)
{
    prePos = transform_.position_;

    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * f);
}

void Player::ApplyDamage(int da)
{
    hp_ -= da;
    return;

}

XMVECTOR Player::GetDirectionVec()
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    return vMove;
}

void Player::CalcRotate()
{
    rotateMove = GetInputMove();
}

void Player::CalcMove()
{
    gradually = stopGradually;

    XMFLOAT3 fMove = GetInputMove();
    rotateMove = fMove;

    XMFLOAT3 move = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + move.x , 0.0f , playerMovement_.z + move.z };

    //MaxSpeed超えていたら正規化・MaxSpeedの値にする
    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));
    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;
        XMStoreFloat3(&playerMovement_, vMove);
    }
}

void Player::CalcNoMove()
{
    XMFLOAT3 move = { 0,0,0 };
    gradually = stopGradually;
    move = { ((move.x - playerMovement_.x) * gradually) , 0.0f , ((move.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + move.x , 0.0f , playerMovement_.z + move.z };
}

void Player::InitAvo()
{
    //動いている場合は大体その方向
    if (pCommand_->CmdWalk()) {
        CalcMove();
        Rotate(avoRotateRatio);
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
    }
    //動いていなくて、ターゲット状態の時は向いている方向の逆に
    else if(pAim_->IsTarget()) {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0);
        rotateMove = XMFLOAT3(-rotateMove.x, -rotateMove.y, -rotateMove.z);
    }
    //動いていなくて、かつターゲットもしていない時は向いている方向に
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
    }

}