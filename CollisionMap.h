#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Engine/Fbx.h"

class Triangle;
class Cell;
class BoxCollider;
class SphereCollider;

class CollisionMap : public GameObject
{
    Cell*** cells_ = 0;

    //範囲内ならtrue / 渡した引数に値が入る
    bool GetCellIndex(XMFLOAT3& pos);

    //指定したCellのポインタを取得 / 範囲外ならnullptr
    Cell* GetCell(XMFLOAT3 pos);

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    bool GetRayCastMinDist(XMFLOAT3 camPos, XMFLOAT3 plaPos, RayCastData* _data, float& minDist);

    //コリジョンマップとの接触判定：めり込んでたら追い出す
    void MapDataVsBox(BoxCollider* collider);
    
    //コリジョンマップとの接触判定：めり込んでたら追い出す
    void MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos);

    //線分上のCellのTriangleとRayの判定当たったら即終了
    void RaySelectCellVsSegment(RayCastData& _data, XMFLOAT3 target);

    void CalcMapWall(XMFLOAT3& _pos, float speed);

    //CollisionMapとの判定時間内からのやつ
    bool IsWall(int x, int z);

};

