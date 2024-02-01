#include "NavigationAI.h"
#include "Stage.h"
#include <queue>

namespace {
	int stageWidth = 0;
	int stageHeight = 0;
	const int DIRX[] = { 0, +1,  0, -1 };
	const int DIRY[] = { -1,  0, +1,  0 };
}

void PathSmoothing(std::vector<XMFLOAT3>& path);

NavigationAI::NavigationAI(Stage* s)
{
	pStage_ = s;
	mapData_ = pStage_->GetMapData();

	stageWidth = (int)mapData_[0].size();
	stageHeight = (int)mapData_.size();
}

std::vector<XMFLOAT3> NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos)
{
	int startX = static_cast<int>(pos.x / floarSize);
	int startZ = static_cast<int>(pos.z / floarSize);
	int targetX = static_cast<int>(target.x / floarSize);
	int targetZ = static_cast<int>(target.z / floarSize);

	//壁に埋まってしまったねぇ
	//startが範囲外・壁の場合
	if (startX < 0 || startX >= stageWidth || startZ < 0 || startZ >= stageHeight ||
		mapData_[startZ][startX] == Stage::MAP::WALL)
	{
		std::vector<XMFLOAT3> none;
		return none;
	}

	//targetが範囲外・壁の場合
	if (targetX < 0 || targetX >= stageWidth || targetZ < 0 || targetZ >= stageHeight ||
		mapData_[targetZ][targetX] == Stage::MAP::WALL)
	{
		std::vector<XMFLOAT3> none;
		return none;
	}

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//探索済みか
	std::vector<std::vector<int>> mapCost(stageHeight, std::vector<int>(stageWidth, 1));			//マップのコストすべて１
	std::vector<std::vector<int>> allCost(stageHeight, std::vector<int>(stageWidth, INT_MAX));	//ノードのコスト
	std::vector<std::vector<int>> value(stageHeight, std::vector<int>(stageWidth, 0));				//スタート地点からの最短距離
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標X
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//そのノードの親ノードの座標Z

	//大きい順にソートするための比較関数
	struct CompareNodes {
		bool operator()(const Node& a, const Node& b) {
			return a.cost > b.cost;
		}
	};
	std::priority_queue<Node, std::vector<Node>, CompareNodes> openList;
	openList.push(Node(startX, startZ, 0));

	while (!openList.empty()) {
		Node current = openList.top();
		openList.pop();

		int x = current.x;
		int z = current.z;

		// 目標に到達したか確認
		if (x == targetX && z == targetZ) {
			// パスを再構築
			std::vector<XMFLOAT3> path;
			while (x != -1 && z != -1) {
				path.push_back(XMFLOAT3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
				int tempX = parentX[x][z];
				int tempY = parentZ[x][z];
				x = tempX;
				z = tempY;
			}

			// 一番後列のデータはstartPosだから削除
			if (path.size() >= 2) {
				path.pop_back();

				PathSmoothing(path);
				return path;
			}
			else {
				// パスが短すぎる場合は目標自体を返す
				path.clear();
				return path;
			}
		}

		closedList[x][z] = true;

		// 隣接するノードを生成
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				// 隣接ノードが範囲内かつ通行可能か確認
				if (newX >= 0 && newX < stageWidth && newZ >= 0 && newZ < stageHeight) {
					if (!closedList[newX][newZ] && mapData_[newZ][newX] == Stage::MAP::FLOAR) {

						//斜め移動の場合は条件をプラス
						if (abs(i) + abs(j) >= 2) {
							if (mapData_[z + j][x] == Stage::MAP::WALL || mapData_[z][x + i] == Stage::MAP::WALL) {
								continue;
							}
						}

						//スコアがallCostより小さければpushする
						//斜め移動はプラス２
						int cellCost = value[x][z] + abs(i) + abs(j);
						int dxValue = targetX - newX;
						int dzValue = targetZ - newZ;
						if (dxValue >= dzValue) cellCost += dxValue;
						else cellCost += dzValue;

						// 新しい経路が現在の最良経路より短いか確認
						if (cellCost < allCost[newX][newZ]) {
							// 隣接ノードの情報を更新
							value[newX][newZ] = value[x][z] + mapCost[newX][newZ];
							parentX[newX][newZ] = x;
							parentZ[newX][newZ] = z;
							allCost[newX][newZ] = cellCost;

							// 隣接ノードをopenListに追加
							openList.push(Node(newX, newZ, cellCost));
						}
					}
				}
			}
		}
	}

	std::vector<XMFLOAT3> none;
	return none;
}

void PathSmoothing(std::vector<XMFLOAT3>& path) {
	const std::vector<XMFLOAT3> prePath = path;
	const float alpha = 0.5f;			// 大きいほど、元のPathに似ているPathができる。　　　　 大きいほど処理が速い
	const float beta = 0.3f;			// 大きいほど、隣接する点間での滑らかさが向上する。　   大きいほど処理が遅い
	const float tolerance = 0.3f;		// 変化量がこの値以下の時平滑化を終了。　　　　　　　　 大きいほど処理が速い
	float change = tolerance;			// パスの位置の変化量

	while (change >= tolerance) {
		change = 0.0f;

		for (int i = 1; i < path.size() - 1; ++i) {
			XMVECTOR currentPathVector = XMLoadFloat3(&path[i]);
			XMVECTOR previousPathVector = XMLoadFloat3(&path[i - 1]);
			XMVECTOR nextPathVector = XMLoadFloat3(&path[i + 1]);
			
			XMVECTOR smoothedPathVector = currentPathVector - alpha * (currentPathVector - XMLoadFloat3(&prePath[i]));
			smoothedPathVector = smoothedPathVector - beta * (2.0f * currentPathVector - previousPathVector - nextPathVector);
			XMStoreFloat3(&path[i], smoothedPathVector);

			//XMVector3LengthEst関数があるらしい、違いよくわからんけど推定値を計算していて、処理がデフォより軽いらしい
			change += XMVectorGetX(XMVector3Length(smoothedPathVector - currentPathVector));
		}
	}
}