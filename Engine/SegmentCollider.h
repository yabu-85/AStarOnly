#pragma once
#include "Collider.h"

//-----------------------------------------------------------
//線分の当たり判定
//-----------------------------------------------------------
class SegmentCollider : public Collider
{
	XMVECTOR vec_; //線の向き（正規化
	
	//Colliderクラスのprivateメンバにアクセスできるようにする
	friend class Collider;

	//接触判定
	//引数：target	相手の当たり判定
	//戻値：接触してればtrue
	bool IsHit(Collider* target) override;

public:
	//コンストラクタ（当たり判定の作成）
	//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：vec 	線の向き
	//引数：range	線の長さ
	SegmentCollider(XMFLOAT3 center, XMVECTOR vec);
	void SetVector(XMVECTOR _vec);

};