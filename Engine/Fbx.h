#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <vector>
#include <string>
#include "Transform.h"

class FbxParts;

//レイキャスト用構造体
struct RayCastData
{
	XMFLOAT3	start;	//レイ発射位置
	XMFLOAT3	dir;	//レイの向きベクトル
	float       dist;	//衝突点までの距離
	BOOL        hit;	//レイが当たったか
	XMFLOAT3 normal;	//法線

	RayCastData() { hit = FALSE; dist = 99999.0f; }
};

//-----------------------------------------------------------
//　FBXファイルを扱うクラス
//　ほとんどの処理は各パーツごとにFbxPartsクラスで行う
//-----------------------------------------------------------
class Fbx
{
	//FbxPartクラスをフレンドクラスにする
	//FbxPartのprivateな関数にもアクセス可
	friend class FbxParts;



	//モデルの各パーツ（複数あるかも）
	std::vector<FbxParts*>	parts_;

	//FBXファイルを扱う機能の本体（FbxImportやFbxSceneを作成するために必要な管理クラスです。
	FbxManager* pFbxManager_;

	//FbxImporter
	//FbxファイルをImportするためのパーサークラスです。
	//ファイルの持つデータをメッシュやカメラなどの情報に分解してくれます。

	//FBXファイルのシーン（Mayaで作ったすべての物体）を扱う
	//Importerによって、分解されたFbxのデータを保存するためのクラスです。
	FbxScene*	pFbxScene_;


	// アニメーションのフレームレート
	FbxTime::EMode	_frameRate;

	//アニメーション速度
	float			_animSpeed;

	//アニメーションの最初と最後のフレーム
	int _startFrame, _endFrame;





	//ノードの中身を調べる
	//引数：pNode		調べるノード
	//引数：pPartsList	パーツのリスト
	void CheckNode(FbxNode* pNode, std::vector<FbxParts*> *pPartsList);




public:
	Fbx();
	~Fbx();

	//ロード
	//引数：fileName	ファイル名
	//戻値：成功したかどうか
	virtual HRESULT Load(std::string fileName);

	//描画
	//引数：World	ワールド行列
	void    Draw(Transform& transform, int type, int frame1, int frame2, float blendFactor);

	//描画
	//引数：World	ワールド行列
	void    Draw(Transform& transform, int type, int frame);

	//解放
	void    Release();

	//任意のボーンの位置を取得
	//引数：boneName	取得したいボーンの位置
	//戻値：ボーンの位置
	XMFLOAT3 GetBonePosition(std::string boneName);

	//任意のアニメーション時のボーンの位置を取得
	XMFLOAT3 GetBoneAnimPosition(std::string boneName, int frame);

	//任意のアニメーション時のボーンの回転を取得
	XMFLOAT3 GetBoneAnimRotate(std::string boneName, int frame);

	//任意のアニメーション時のボーンの回転Matrixを取得
	XMMATRIX GetBoneAnimRotateMatrix(std::string boneName, int frame);

	//レイキャスト（レイを飛ばして当たり判定）
	//引数：data	必要なものをまとめたデータ
	void RayCast(RayCastData *data);

	std::vector<FbxParts*> GetFbxParts() { return parts_; };

};

