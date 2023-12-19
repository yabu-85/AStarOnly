#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>
#include "Engine/CsvReader.h"
#include "CollisionMap.h"

namespace {
    //デバッグ用
    bool drawCell = true;
}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), startPos_(0,0,0), mapSizeX_(0), mapSizeZ_(0)
{
    for (int i = 0; i < MAX + RMAX; i++) hModel_[i] = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    std::string fileName[] = { "StageT1", "StageT2", "RayStageT1", "RayStageT2" };
    for (int i = 0; i < MAX + RMAX; i++) {
        hModel_[i] = Model::Load("Model/Stage/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }
    
    CreatStage();
}

void Stage::Update()
{
    //デバッグ用
    if (Input::IsKeyDown(DIK_E)) drawCell = !drawCell;

}

void Stage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        trans.scale_ = intersectDatas_.at(i).scale;
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray用のワイヤー表示
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum + MAX;
            Transform trans;
            trans.position_ = intersectDatas_.at(i).position;
            trans.scale_ = intersectDatas_.at(i).scale;
            Model::SetTransform(handle, trans);
            Model::Draw(handle, 2);
        }
    }
}

void Stage::Release()
{
}

XMFLOAT3 Stage::GetPlayerStartPos()
{
    return XMFLOAT3(startPos_.x * floarSize + (floarSize / 2), 0.0f, startPos_.z * floarSize + (floarSize / 2));
}

void Stage::ResetStage()
{
}

//HADESみたいに作ろう：CSVを読み込みで作成
void Stage::CreatStage()
{
    //CSVファイル読み込み
    CsvReader csv;
    csv.Load("Csv/Map2.csv");

    //ステージの幅と高さを設定
    mapSizeX_ = (int)csv.GetWidth();
    mapSizeZ_ = (int)csv.GetHeight();

    //サイズをマップの大きさにする
    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++) mapData_[i].resize(mapSizeX_, FLOAR);

    //CSVデータをテーブルに格納
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            int data = csv.GetValue(x, z);
            if (data == 10)
            {
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f / smallSize, 1.0f / smallSize) });
                startPos_ = XMFLOAT3((float)x, 0.0f, (float)z);
            }
            else if (data == 1)
            {
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f / smallSize, 1.0f / smallSize) });
            }
            else if (data == 2)
            {
                mapData_[z][x] = WALL;
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f * 0.1f / smallSize, 1.0f / smallSize) });
                intersectDatas_.push_back({ hModel_[T2], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f * 0.1f / smallSize, 1.0f / smallSize) });
            }
        }
    }
}

XMFLOAT3 Stage::GetRandomFloarPosition()
{
    struct Cell
    {
        float x, z;
    };
    std::vector<Cell> posList;
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            if (mapData_[z][x] == Stage::MAP::FLOAR) {
                Cell cell;
                cell.x = (float)x;
                cell.z = (float)z;
                posList.push_back(cell);
            }
        }
    }

    int index = rand() % posList.size();
    return XMFLOAT3(posList.at(index).x * floarSize + (floarSize / 2), 0.0f, posList.at(index).z * floarSize + (floarSize / 2));
}
