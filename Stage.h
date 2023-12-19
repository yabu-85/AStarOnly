#pragma once
#include "Engine/GameObject.h"
#include <vector>

struct IntersectData {
    int hModelNum;
    XMFLOAT3 position;
    XMFLOAT3 scale;

    IntersectData() : hModelNum(-1), position{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } { }
    IntersectData(int num, XMFLOAT3 pos) : hModelNum(num), position(pos), scale{1.0f, 1.0f, 1.0f} { }
    IntersectData(int num, XMFLOAT3 pos, XMFLOAT3 sca) : hModelNum(num), position(pos), scale(sca) { }
};

//デバッグ用
const float floarSize = 5.0f;
const float smallSize = 1.0f;

class Stage : public GameObject
{
public:
    enum StageNum {
        T1 = 0,
        T2,
        MAX,
    };

    enum RayStageNum {
        RT1 = 0,
        RT2,
        RMAX,
    };

private:
    int hModel_[MAX + RMAX];
    std::vector<IntersectData> intersectDatas_; //ローポリのデータ集

    int mapSizeX_;
    int mapSizeZ_;
    XMFLOAT3 startPos_;
    std::vector<std::vector<int>> mapData_;

public:
    enum MAP {
        FLOAR,
        WALL,
    };

    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    std::vector<IntersectData> GetIntersectDatas() { return intersectDatas_; };
    std::vector<std::vector<int>> GetMapData() { return mapData_; };
    XMFLOAT3 GetPlayerStartPos();
    XMFLOAT3 GetRandomFloarPosition();

    void ResetStage();
    void CreatStage();

};