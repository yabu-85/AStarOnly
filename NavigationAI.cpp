#include "NavigationAI.h"
#include "Stage.h"
#include <queue>

namespace {
	int stageWidth = 0;
	int stageHeight = 0;
	const int DIRX[] = { 0, +1,  0, -1 };
	const int DIRY[] = { -1,  0, +1,  0 };
}

bool NavigationAI::IsInWall(int x, int z)
{
	if (mapData_[z][x] == Stage::MAP::WALL) return true;
	return false;
}

bool NavigationAI::IsOutRange(int x, int z)
{
	if (x < 0 || x >= stageWidth || z < 0 || z >= stageHeight) return true;
	return false;
}

std::vector<XMFLOAT3> NavigationAI::CalcExtractPath(int x, int z, std::vector<std::vector<int>> pX, std::vector<std::vector<int>> pZ)
{
	//�p�X���č\�z
	std::vector<XMFLOAT3> path;
	while (x != -1 && z != -1) {
		path.push_back(XMFLOAT3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
		int tempX = pX[x][z];
		int tempY = pZ[x][z];
		x = tempX;
		z = tempY;
	}

	//��Ԍ��̃f�[�^��startPos������폜
	if (path.size() >= 2) {
		path.pop_back();
		PathSmoothing(path);
		return path;
	}
	//�p�X���Z������ꍇ�͉��������ĂȂ���Ԃ�
	else {
		path.clear();
		return path;
	}
}


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

	if (IsOutRange(startX, startZ) || IsOutRange(targetX, targetZ) || IsInWall(startX, startZ) || IsInWall(targetX, targetZ))
	{
		std::vector<XMFLOAT3> none;
		return none;
	}

	std::vector<std::vector<bool>> closedList(stageWidth, std::vector<bool>(stageHeight, false));	//�T���ς݂�
	std::vector<std::vector<int>> mapCost(stageHeight, std::vector<int>(stageWidth, 1));			//�}�b�v�̃R�X�g���ׂĂP
	std::vector<std::vector<int>> allCost(stageHeight, std::vector<int>(stageWidth, INT_MAX));	//�m�[�h�̃R�X�g
	std::vector<std::vector<int>> value(stageHeight, std::vector<int>(stageWidth, 0));				//�X�^�[�g�n�_����̍ŒZ����
	std::vector<std::vector<int>> parentX(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WX
	std::vector<std::vector<int>> parentZ(stageWidth, std::vector<int>(stageHeight, -1));			//���̃m�[�h�̐e�m�[�h�̍��WZ

	//�傫�����Ƀ\�[�g���邽�߂̔�r�֐�
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

		//�ڕW�ɓ��B�������m�F���ē��B�����Ȃ�v�Z���Č��ʏo��
		if (x == targetX && z == targetZ) 
			return CalcExtractPath(x, z, parentX, parentZ);

		closedList[x][z] = true;

		//�אڂ���m�[�h�𐶐�
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int newX = x + i;
				int newZ = z + j;

				//�אڃm�[�h���͈͓����ʍs�\���m�F
				if (!IsOutRange(newX, newZ) && !closedList[newX][newZ] && !IsInWall(newX, newZ)){

					//�΂߈ړ��̏ꍇ�͏������v���X���ėǂ���Όv�Z����
					if (abs(i) + abs(j) >= 2 && (IsInWall(x, z+j) || IsInWall(x+i, z)) )
						continue;

					//�X�R�A��allCost��菬�������push����
					int cellCost = value[x][z] + abs(i) + abs(j);
					int dxValue = targetX - newX;
					int dzValue = targetZ - newZ;
					if (dxValue >= dzValue) cellCost += dxValue;
					else cellCost += dzValue;

					// �V�����o�H�����݂̍ŗǌo�H���Z�����m�F
					if (cellCost < allCost[newX][newZ]) {
						// �אڃm�[�h�̏����X�V
						value[newX][newZ] = value[x][z] + mapCost[newX][newZ];
						parentX[newX][newZ] = x;
						parentZ[newX][newZ] = z;
						allCost[newX][newZ] = cellCost;

						// �אڃm�[�h��openList�ɒǉ�
						openList.push(Node(newX, newZ, cellCost));
					}
				}
			}
		}
	}

	std::vector<XMFLOAT3> none;
	return none;
}

void NavigationAI::PathSmoothing(std::vector<XMFLOAT3>& path) {
	const std::vector<XMFLOAT3> prePath = path;
	const float alpha = 0.5f;			// �傫���قǁA����Path�Ɏ��Ă���Path���ł���B�@�@�@�@ �傫���قǏ���������
	const float beta = 0.3f;			// �傫���قǁA�אڂ���_�Ԃł̊��炩�������シ��B�@   �傫���قǏ������x��
	const float tolerance = 0.3f;		// �ω��ʂ����̒l�ȉ��̎����������I���B�@�@�@�@�@�@�@�@ �傫���قǏ���������
	float change = tolerance;			// �p�X�̈ʒu�̕ω���

	while (change >= tolerance) {
		change = 0.0f;

		for (int i = 1; i < path.size() - 1; ++i) {
			XMVECTOR currentPathVector = XMLoadFloat3(&path[i]);
			XMVECTOR previousPathVector = XMLoadFloat3(&path[i - 1]);
			XMVECTOR nextPathVector = XMLoadFloat3(&path[i + 1]);
			
			XMVECTOR smoothedPathVector = currentPathVector - alpha * (currentPathVector - XMLoadFloat3(&prePath[i]));
			smoothedPathVector = smoothedPathVector - beta * (2.0f * currentPathVector - previousPathVector - nextPathVector);
			XMStoreFloat3(&path[i], smoothedPathVector);

			//XMVector3LengthEst�֐�������炵���A�Ⴂ�悭�킩��񂯂ǐ���l���v�Z���Ă��āA�������f�t�H���y���炵��
			change += XMVectorGetX(XMVector3Length(smoothedPathVector - currentPathVector));
		}
	}
}