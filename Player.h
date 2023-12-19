#pragma once
#include "Character.h"

class Aim;
class StateManager;
class PlayerCommand;

class Player : public Character
{
    int hModel_[2];
    int money_;
    float moveSpeed_;           //�ړ��X�s�[�h
    float rotateRatio_;         //��]�̔䗦
    XMFLOAT3 playerMovement_;   //���̈ړ���(Key�̏�񂾂�

    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerCommand* pCommand_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Rotate();
    void Rotate(float ratio);

    XMFLOAT3 GetInputMove();        //Input�̈ړ��ʎ擾
    void FrontMove(float f = 1.0f); //�O�ւ̈ړ�
    void Move(float f = 1.0f);      //���ۂ̈ړ�
    void CalcRotate();              //�ړ��ʂ͍l�����Ȃ���]�݂̂̌v�Z
    void CalcMove();                //Input���l���������
    void CalcNoMove();              //Input�l�����ĂȂ��A�����Ƃ��̌v�Z�p

    void ResetKeyMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    XMFLOAT3 GetKeyMovement() { return playerMovement_; }
    XMVECTOR GetDirectionVec();     //�����Ă�������i���K���ς�

    void ApplyDamage(int da) override;
    void InitAvo();
    void AddMoney(int num) { money_ += num; }
    int GetModelHandle() { return hModel_[0]; }

    PlayerCommand* GetCommand() { return pCommand_; }
    Aim* GetAim() { return pAim_; }

};
