#include "SegmentCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

//コンストラクタ（当たり判定の作成）
//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
//引数：size	当たり判定のサイズ
SegmentCollider::SegmentCollider(XMFLOAT3 center, XMVECTOR vec)
{
	center_ = center;
	type_ = COLLIDER_SEGMENT;

	float size = XMVectorGetX(XMVector3Length(vec));
	size_ = XMFLOAT3(size, size, size);
	vec_ = XMVector3Normalize(vec);
	
	//リリース時は判定枠は表示しない
#ifdef _DEBUG
	//テスト表示用判定枠
	hDebugModel_ = Model::Load("DebugCollision/sphereCollider.fbx");
#endif
}

void SegmentCollider::SetVector(XMVECTOR _vec)
{
	float size = XMVectorGetX(XMVector3Length(_vec));
	size_ = XMFLOAT3(size, size, size); 
	vec_ = XMVector3Normalize(_vec);
}

//接触判定
//引数：target	相手の当たり判定
//戻値：接触してればtrue
bool SegmentCollider::IsHit(Collider* target)
{
	if (target->type_ == COLLIDER_BOX)
		return IsHitBoxVsSegment((BoxCollider*)target, this);
	else if (target->type_ == COLLIDER_CIRCLE)
		return IsHitCircleVsSegment((SphereCollider*)target, this);
	else 
		return IsHitSegmentVsSegment((SegmentCollider*)target, this);
}