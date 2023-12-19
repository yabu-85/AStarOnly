#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class Stage;

class NavigationAI
{
	Stage* pStage_;
	std::vector<std::vector<int>> mapData_;

	struct Node {
		int x, z;
		int cost;
	};

    // openListからコストが最小のノードを取得
    int GetMinCostNodeIndex(std::vector<Node>& openList);

public:
	NavigationAI(Stage* s);

	//AStarの縦横移動のみ
	std::vector<XMFLOAT3> Navi(XMFLOAT3 target, XMFLOAT3 pos);
	
	//AStarの斜め移動あり
	std::vector<XMFLOAT3> NaviDiagonal(XMFLOAT3 target, XMFLOAT3 pos);

};

