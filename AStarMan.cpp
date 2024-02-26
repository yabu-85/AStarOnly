#include "AStarMan.h"
#include "Engine/Model.h"
#include "Stage.h"
#include "Engine/Global.h"
#include "MoveAction.h"

//デバッグ用
#include <vector>
#include "Engine/Input.h"
#include "Player.h"
namespace {
	int randTime = 60;
}

AStarMan::AStarMan(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), pAstarMoveAction_(nullptr)
{
}

AStarMan::~AStarMan()
{
}

void AStarMan::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/SphereCollider.fbx");
	assert(hModel_ >= 0);
	
	aimTargetPos_ = 1.0f;

	Stage* pStage = (Stage*)FindObject("Stage");
	XMFLOAT3 startPos = pStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);

	pAstarMoveAction_ = new AstarMoveAction(this, 0.03f, 0.1f);
	pAstarMoveAction_->Initialize();
	pAstarMoveAction_->SetTarget(startPos);

}

void AStarMan::Update()
{
	if (pAstarMoveAction_->IsInRange() && rand() % randTime == 0) {
		Player* pPlayer = (Player*)FindObject("Player");
		XMFLOAT3 pPos = pPlayer->GetPosition();
		pAstarMoveAction_->SetTarget(pPos);

//		Stage* pStage = (Stage*)FindObject("Stage");
//		pAstarMoveAction_->SetTarget(pStage->GetRandomFloarPosition());
	}

	pAstarMoveAction_->Update();

}

void AStarMan::Draw()
{
	Transform draw = transform_;
	draw.position_.y += 1.0f;
	Model::SetTransform(hModel_, draw);
	Model::Draw(hModel_);

	if (Input::IsKey(DIK_F)) {
		Transform target;
		target.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
		std::vector<XMFLOAT3> targetList = pAstarMoveAction_->GetTarget();
		if (targetList.empty()) return;
		for (auto pos : targetList) {
			target.position_ = XMFLOAT3(pos.x * floarSize + floarSize / 2.0f, pos.y, pos.z * floarSize + floarSize / 2.0f);
			target.position_.y += 1.0f;
			Model::SetTransform(hModel_, target);
			Model::Draw(hModel_);
		}
	}
}

void AStarMan::Release()
{
	SAFE_DELETE(pAstarMoveAction_);

}
