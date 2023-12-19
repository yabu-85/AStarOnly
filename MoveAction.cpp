#include "MoveAction.h"
#include "Player.h"
#include "CollisionMap.h"
#include "GameManager.h"
#include "EnemyManager.h"
#include "NavigationAI.h"
#include "Stage.h"
#include "EnemyBase.h"

MoveAction::MoveAction(Character* obj, float speed, float range)
	: BaseAction(obj), isInRange_(false), moveSpeed_(speed), moveRange_(range), targetPos_(0, 0, 0)
{
}

void MoveAction::Update()
{
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMove = vTar - vPos;
	float currentSpeed = XMVectorGetX(XMVector3Length(vTar - vPos));
	if(currentSpeed > moveSpeed_) vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//Target位置ついた
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		isInRange_ = true;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	pCharacter_->SetPosition(pos);
	isInRange_ = false;;
}

//------------------------------Astar----------------------

AstarMoveAction::AstarMoveAction(Character* obj, float speed, float range) : MoveAction(obj, speed, range), isOutEndTarget_(false)
{
}

void AstarMoveAction::Update()
{
	isInRange_ = false;

	//移動終了した
	if (targetList_.empty()) {
		isInRange_ = true;
		isOutEndTarget_ = false;
		return;
	}
	
	XMVECTOR half = XMVectorSet(floarSize / 2.0f, 0.0f, floarSize / 2.0f, 0.0f);
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetList_.back()) * floarSize + half;

	//Target離れすぎたから更新
	XMVECTOR endTarget = XMLoadFloat3(&targetList_.front()) * floarSize + half;
	const float endTargetRange = 10.0f;
	if (endTargetRange < XMVectorGetX(XMVector3Length(endTarget - vTar))) {
		isOutEndTarget_ = true;
	}

	XMVECTOR vMove = vTar - vPos;
	float currentSpeed = XMVectorGetX(XMVector3Length(vMove));
	if (currentSpeed > moveSpeed_ * floarSize) vMove = XMVector3Normalize(vMove) * (moveSpeed_ * floarSize);

	//Target位置ついた：カクカクしないように再起処理する
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		targetList_.pop_back();
		Update();
		return;
	}

	XMStoreFloat3(&pos, vPos + vMove);

	//壁とのあたり判定してからポジションセット
	CollisionMap* pMap = (CollisionMap*)pCharacter_->FindObject("CollisionMap");
	pMap->CalcMapWall(pos, moveSpeed_);
	pCharacter_->SetPosition(pos);
}

void AstarMoveAction::SetTarget(XMFLOAT3 target)
{
	targetList_ = GameManager::GetNavigationAI()->NaviDiagonal(target, pCharacter_->GetPosition());
	isOutEndTarget_ = false;
}