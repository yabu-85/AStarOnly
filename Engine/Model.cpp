#include "Global.h"
#include "Model.h"

//3Dモデル（FBXファイル）を管理する
namespace Model
{
	//ロード済みのモデルデータ一覧
	std::vector<ModelData*>	_datas;

	//初期化
	void Initialize()
	{
		AllRelease();
	}

	//モデルをロード
	int Load(std::string fileName)
	{
		ModelData* pData = new ModelData;


		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
			{
				pData->pFbx = _datas[i]->pFbx;
				isExist = true;
				break;
			}
		}

		//新たにファイルを開く
		if (isExist == false)
		{
			pData->pFbx = new Fbx;
			if (FAILED(pData->pFbx->Load(fileName)))
			{
				//開けなかった
				SAFE_DELETE(pData->pFbx);
				SAFE_DELETE(pData);
				return -1;
			}

			//無事開けた
			pData->fileName = fileName;
		}


		//使ってない番号が無いか探す
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] == nullptr)
			{
				_datas[i] = pData;
				return i;
			}
		}

		//新たに追加
		_datas.push_back(pData);
		return (int)_datas.size() - 1;
	}



	//描画
	void Draw(int handle, int type)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}

		//アニメーションを進める
		if (!_datas[handle]->isAimeStop) {
			_datas[handle]->nowFrame += _datas[handle]->animSpeed;
		
			//最後までアニメーションしたら戻す
			if (_datas[handle]->nowFrame > (float)_datas[handle]->endFrame) {
				_datas[handle]->nowFrame = (float)_datas[handle]->startFrame;
			}
		}
		
		if (_datas[handle]->pFbx)
		{
			/*
			if (_datas[handle]->isBlending) {
				_datas[handle]->pFbx->Draw(_datas[handle]->transform, type, (int)_datas[handle]->nowFrame, (int)_datas[handle]->blendFrame, _datas[handle]->blendWeight);
				return;
			}
			*/

			_datas[handle]->pFbx->Draw(_datas[handle]->transform, type, (int)_datas[handle]->nowFrame);
		}
	}


	//任意のモデルを開放
	void Release(int handle)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}

		//同じモデルを他でも使っていないか
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && i != handle && _datas[i]->pFbx == _datas[handle]->pFbx)
			{
				isExist = true;
				break;
			}
		}

		//使ってなければモデル解放
		if (isExist == false )
		{
			SAFE_DELETE(_datas[handle]->pFbx);
		}


		SAFE_DELETE(_datas[handle]);
	}


	//全てのモデルを解放
	void AllRelease()
	{
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] != nullptr)
			{
				Release(i);
			}
		}
		_datas.clear();
	}

	//アニメーションのフレーム数をセット
	void SetBlendingAnimFrame(int handle, int startFrame1, int endFrame1, int startFrame2, float animSpeed, float blendWeight)
	{
		_datas[handle]->isAimeStop = false;
		_datas[handle]->SetBlendingAnimFrame(startFrame1, endFrame1, startFrame2, animSpeed, blendWeight);
	}

	//アニメーションのフレーム数をセット
	void SetAnimFrame(int handle, int startFrame, int endFrame, float animSpeed)
	{
		_datas[handle]->isAimeStop = false;
		_datas[handle]->SetAnimFrame(startFrame, endFrame, animSpeed);
	}

	void SetBlendFactor(int handle, float weight)
	{
		_datas[handle]->blendWeight = weight;
	}

	void SetAnimeStop(int handle, bool b)
	{
		_datas[handle]->isAimeStop = b;
	}

	bool IsAnimeStop(int handle)
	{
		return _datas[handle]->isAimeStop;
	}

	//現在のアニメーションのフレームを取得
	int GetAnimFrame(int handle)
	{
		return (int)_datas[handle]->nowFrame;
	}


	//任意のボーンの位置を取得
	XMFLOAT3 GetBonePosition(int handle, std::string boneName)
	{
		XMFLOAT3 pos = _datas[handle]->pFbx->GetBonePosition(boneName);
		XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos), _datas[handle]->transform.GetWorldMatrix());
		XMStoreFloat3(&pos, vec);
		return pos;
	}

	XMFLOAT3 GetBoneAnimPosition(int handle, std::string boneName)
	{
		XMFLOAT3 pos = _datas[handle]->pFbx->GetBoneAnimPosition(boneName, (int)_datas[handle]->nowFrame); //相対座標（ボーンの中心からの位置）
		XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos), _datas[handle]->transform.GetWorldMatrix()); //posをワールドマトリックスで計算する
		XMStoreFloat3(&pos, vec);
		return pos;
	}

	XMFLOAT3 GetBoneAnimRotate(int handle, std::string boneName)
	{
		return _datas[handle]->pFbx->GetBoneAnimRotate(boneName, (int)_datas[handle]->nowFrame); //相対座標（ボーンの中心からの位置）
	}

	XMMATRIX GetBoneAnimRotateMatrix(int handle, std::string boneName)
	{
		return _datas[handle]->pFbx->GetBoneAnimRotateMatrix(boneName, (int)_datas[handle]->nowFrame); //相対座標（ボーンの中心からの位置）
	}


	//ワールド行列を設定
	void SetTransform(int handle, Transform & transform)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		_datas[handle]->transform = transform;
	}


	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle)
	{
		return _datas[handle]->transform.GetWorldMatrix();
	}

	Fbx* GetFbx(int handle)
	{
		return _datas[handle]->pFbx;
	}


	//レイキャスト（レイを飛ばして当たり判定）
	void RayCast(int handle, RayCastData *data)
	{
			XMFLOAT3 target = Transform::Float3Add(data->start, data->dir);
			XMMATRIX matInv = XMMatrixInverse(nullptr, _datas[handle]->transform.GetWorldMatrix());
			XMVECTOR vecStart = XMVector3TransformCoord(XMLoadFloat3(&data->start), matInv);
			XMVECTOR vecTarget = XMVector3TransformCoord(XMLoadFloat3(&target), matInv);
			XMVECTOR vecDir = vecTarget - vecStart;

			XMStoreFloat3(&data->start, vecStart);
			XMStoreFloat3(&data->dir, vecDir);

			_datas[handle]->pFbx->RayCast(data); 
	}
}