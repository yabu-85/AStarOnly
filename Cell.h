#pragma once
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class BoxCollider;
class SphereCollider;

class Cell
{
	//¶‰œ‰º‚ÌÀ•W
	XMFLOAT3 position_;

	//cube‚Ìˆê•Ó‚Ì’·‚³
	float length_ = 0;

	//’¸“_ˆÊ’u posision
	XMFLOAT3 verPos_[8];

	//‚±‚ÌCELL‚É‘®‚·‚éOŠpƒ|ƒŠƒSƒ“
	std::vector<Triangle*>Triangles;
	int NumTriangles = 0;

public:
	Cell();

	void SetPosLeng(XMFLOAT3 pos, float leng);

	//OŠpƒ|ƒŠƒSƒ“‚ªAAB‚ÉG‚ê‚Ä‚¢‚½‚ç‚±‚ÌCELL‚É“o˜^
	bool SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

	//Getter
	XMFLOAT3 GetPosision() { return position_; };
	std::vector<Triangle*>& GetTriangles() { return Triangles; };

	//“–‚½‚Á‚½ê‡‚ÌÅ¬‹——£‚ğ•Ô‚·
	bool SegmentVsTriangle(RayCastData* _data, float& minDist);

	void MapDataVsBox(BoxCollider* collider);
	void MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos);

};


class CellBox : public GameObject
{
	int hModel_ = -1;

public:
	CellBox(GameObject* parent) : GameObject(parent, "CollisionMap"), hModel_(-1) {};
	~CellBox() {};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};