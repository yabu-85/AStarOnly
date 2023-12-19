#include "BoxCollider.h"
#include "SphereCollider.h"
#include "SegmentCollider.h"
#include "GameObject.h"
#include "Model.h"
#include "Transform.h"

//コンストラクタ
Collider::Collider():
	pGameObject_(nullptr), center_(0.0f, 0.0f, 0.0f), hDebugModel_(-1), size_(0.0f, 0.0f, 0.0f), type_(COLLIDER_MAX)
{
}

//デストラクタ
Collider::~Collider()
{
}

//箱型同士の衝突判定
//引数：boxA	１つ目の箱型判定
//引数：boxB	２つ目の箱型判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB)
{

	XMFLOAT3 boxPosA = Transform::Float3Add(boxA->pGameObject_->GetWorldPosition(), boxA->center_);
	XMFLOAT3 boxPosB = Transform::Float3Add(boxB->pGameObject_->GetWorldPosition(), boxB->center_);

	if ((boxPosA.x + boxA->size_.x / 2) > (boxPosB.x - boxB->size_.x / 2) &&
		(boxPosA.x - boxA->size_.x / 2) < (boxPosB.x + boxB->size_.x / 2) &&
		(boxPosA.y + boxA->size_.y / 2) > (boxPosB.y - boxB->size_.y / 2) &&
		(boxPosA.y - boxA->size_.y / 2) < (boxPosB.y + boxB->size_.y / 2) &&
		(boxPosA.z + boxA->size_.z / 2) > (boxPosB.z - boxB->size_.z / 2) &&
		(boxPosA.z - boxA->size_.z / 2) < (boxPosB.z + boxB->size_.z / 2))
	{
		return true;
	}
	return false;
}

//箱型と球体の衝突判定
//引数：box	箱型判定
//引数：sphere	２つ目の箱型判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere)
{
	XMFLOAT3 circlePos = Transform::Float3Add(sphere->pGameObject_->GetWorldPosition(), sphere->center_);
	XMFLOAT3 boxPos = Transform::Float3Add(box->pGameObject_->GetWorldPosition(), box->center_);

	if (circlePos.x > boxPos.x - box->size_.x - sphere->size_.x &&
		circlePos.x < boxPos.x + box->size_.x + sphere->size_.x &&
		circlePos.y > boxPos.y - box->size_.y - sphere->size_.x &&
		circlePos.y < boxPos.y + box->size_.y + sphere->size_.x &&
		circlePos.z > boxPos.z - box->size_.z - sphere->size_.x &&
		circlePos.z < boxPos.z + box->size_.z + sphere->size_.x )
	{
		return true;
	}

	return false;
}

//箱型と線分の衝突判定
//引数：box	 箱型判定
//引数：seg  線分
//戻値：接触していればtrue
bool Collider::IsHitBoxVsSegment(BoxCollider* box, SegmentCollider* seg)
{

	return false;
}

//球体同士の衝突判定
//引数：circleA	１つ目の球体判定
//引数：circleB	２つ目の球体判定
//戻値：接触していればtrue
bool Collider::IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB)
{
	XMFLOAT3 centerA = circleA->center_;
	XMFLOAT3 positionA = circleA->pGameObject_->GetWorldPosition();
	XMFLOAT3 centerB = circleB->center_;
	XMFLOAT3 positionB = circleB->pGameObject_->GetWorldPosition();

	XMVECTOR v = (XMLoadFloat3(&centerA) + XMLoadFloat3(&positionA))
				- (XMLoadFloat3(&centerB) + XMLoadFloat3(&positionB));

	if (XMVector3Length(v).m128_f32[0] <= circleA->size_.x + circleB->size_.x)
	{
		return true;
	}

	return false;
}

//球体と線分の衝突判定
//引数：circle 球体判定
//引数：seg    線分
//戻値：接触していればtrue
bool Collider::IsHitCircleVsSegment(SphereCollider* circle, SegmentCollider* seg)
{
	//球体の中心座標
	XMFLOAT3 f = circle->pGameObject_->GetWorldPosition();
	XMVECTOR cCenter = XMLoadFloat3(&f) + XMLoadFloat3(&circle->center_);

	//線分の始点と終点座標
	f = seg->pGameObject_->GetWorldPosition();
	XMVECTOR sStart = XMLoadFloat3(&f) + XMLoadFloat3(&seg->center_);
	XMVECTOR sEnd = sStart + seg->vec_ * seg->size_.x;
	
	//線分の終点と球の距離が円の範囲内なら当たってる
	XMVECTOR vP = cCenter - sEnd;
	if (circle->size_.x > XMVectorGetX(XMVector3Length(vP))) return true;

	//線分の始点と球の距離が円の範囲内なら当たってる
	vP = cCenter - sStart;
	if (circle->size_.x > XMVectorGetX(XMVector3Length(vP))) return true;

	//内積が0より大きく、線分ベクトルの大きさより小さいなら
	float dot = XMVectorGetX(XMVector3Dot(seg->vec_, vP));
	if (dot > 0 && dot < seg->size_.x) {
		//dotの長さのベクトル
		XMVECTOR vec = seg->vec_ * dot;

		//球体から一番近いsegmentの点と球体までの距離が球体の半径の２乗より小さければ当たってる
		float range = XMVectorGetX(XMVector3Length(vP - vec));
		if (circle->size_.x > range) return true;
	}
	return false;
	
}


//線分同士の衝突判定
//引数：SegA	１つ目の線分
//引数：SegmB	２つ目の線分
//戻値：接触していればtrue
bool Collider::IsHitSegmentVsSegment(SegmentCollider* segA, SegmentCollider* segB)
{
	return false;
}

//テスト表示用の枠を描画
//引数：position	オブジェクトの位置
void Collider::Draw(XMFLOAT3 position)
{
	Transform transform;
	transform.position_ = XMFLOAT3(position.x + center_.x, position.y + center_.y, position.z + center_.z);
	transform.scale_ = size_;
	transform.Calclation();
	Model::SetTransform(hDebugModel_, transform);
	Model::Draw(hDebugModel_, 4);
}
