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
		Node(int x, int z, int cost) : x(x), z(z), cost(cost) {}
	};

	bool IsInWall(int x, int z);
	bool IsOutRange(int x, int z);
	std::vector<XMFLOAT3> CalcExtractPath(int x, int z, std::vector<std::vector<int>> pX, std::vector<std::vector<int>> pZ);
	void PathSmoothing(std::vector<XMFLOAT3>& path);

public:
	NavigationAI(Stage* s);
	std::vector<XMFLOAT3> Navi(XMFLOAT3 target, XMFLOAT3 pos);
	
};

