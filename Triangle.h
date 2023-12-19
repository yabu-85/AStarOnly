#pragma once
#include <DirectXMath.h>
#include "Engine/Fbx.h"

using namespace DirectX;
class BoxCollider;
class SphereCollider;

class Triangle
{
	//３頂点の座標
	XMVECTOR position_[3];
	
	//法線ベクトル
	XMVECTOR normal_;

public:
	void CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3);
	XMFLOAT3* GetPosition();

	void RayCast(RayCastData* data);
	bool TestSphereTriangle(SphereCollider* collider);

};

