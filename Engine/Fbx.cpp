#include "Fbx.h"
#include "Direct3D.h"
#include "FbxParts.h"



Fbx::Fbx():_animSpeed(0)
{
}


Fbx::~Fbx()
{
	for (int i = 0; i < parts_.size(); i++)
	{
		delete parts_[i];
	}
	parts_.clear();

	pFbxScene_->Destroy();
	pFbxManager_->Destroy();
}

HRESULT Fbx::Load(std::string fileName)
{
	// FBX�̓ǂݍ���
	pFbxManager_ = FbxManager::Create();
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	FbxString FileName(fileName.c_str());
	FbxImporter *fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	if (!fbxImporter->Initialize(FileName.Buffer(), -1, pFbxManager_->GetIOSettings()))
	{
		//���s
		return E_FAIL;
	}
	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();

	// �A�j���[�V�����̃^�C�����[�h�̎擾(�t���[�����[�g�Ƃ��́i60fps)�j
	_frameRate = pFbxScene_->GetGlobalSettings().GetTimeMode();


	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//�J�����g�f�B���N�g�����t�@�C�����������ꏊ�ɕύX
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);
	SetCurrentDirectory(dir);

	//FbxScene�I�u�W�F�N�g->���̃I�u�W�F�N�g�̒���FBX�t�@�C���̂��ׂĂ̏�񂪊i�[����Ă��܂��B
	//FBX�͏��̋���ȃc���[�\���ɂȂ��Ă��܂��B���̃c���[�͊�{���ׂāuFbxNode�v�Ƃ����m�[�h����{�ƂȂ��Ă��܂�
	//����ȃc���[�̃g�b�v�́u���[�g�m�[�h�v�ƌĂ΂�܂��B�����̉��ɂ������񂪂Ԃ牺�����Ă��܂�
	
	//���[�g�m�[�h���擾����
	FbxNode* rootNode = pFbxScene_->GetRootNode();

	//�����̎q���̐��𒲂ׂ�
	int childCount = rootNode->GetChildCount();

	//1���`�F�b�N
	for (int i = 0; childCount > i; i++)
	{
		CheckNode(rootNode->GetChild(i), &parts_);
	}

	//�J�����g�f�B���N�g�������̈ʒu�ɖ߂�
	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;
}

void Fbx::CheckNode(FbxNode * pNode, std::vector<FbxParts*>* pPartsList)
{
	//���̃m�[�h�ɂ̓��b�V����񂪓����Ă��邾�낤���H
	//FbxNodeAttribute = ����͂��̃m�[�h����̓I�ɂǂ��������������Ă��邩��\���I�u�W�F�N�g�i�N���X�̒�`������Ε������
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr != nullptr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//�p�[�c��p��
		FbxParts* pParts = new FbxParts;
		pParts->Init(pNode);

		//�p�[�c���𓮓I�z��ɒǉ�
		pPartsList->push_back(pParts);
	}


	//�q�m�[�h�ɂ��f�[�^�����邩���I�I
	{
		//�q���̐��𒲂ׂ�
		int childCount = pNode->GetChildCount();

		//��l���`�F�b�N
		for (int i = 0; i < childCount; i++)
		{
			CheckNode(pNode->GetChild(i), pPartsList);
		}
	}
}


void Fbx::Release()
{

}

XMFLOAT3 Fbx::GetBonePosition(std::string boneName)
{
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePosition(boneName, &position))
			break;
	}


	return position;
}

XMFLOAT3 Fbx::GetBoneAnimPosition(std::string boneName, int frame)
{
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);

	FbxTime time;
	time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);

	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePosition(boneName, time, &position))
			break;
	}

	return position;
}

XMFLOAT3 Fbx::GetBoneAnimRotate(std::string boneName, int frame)
{
	XMFLOAT3 rotate;
	FbxTime time;
	time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);

	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBoneRotate(boneName, time, &rotate))
			break;
	}

	return rotate;
}

XMMATRIX Fbx::GetBoneAnimRotateMatrix(std::string boneName, int frame)
{
	XMMATRIX rotate;
	FbxTime time;
	time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);

	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBoneRotateMatrix(boneName, time, &rotate))
			break;
	}

	return rotate;
}

void Fbx::Draw(Transform& transform, int type, int frame1, int frame2, float blendFactor)
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
	Direct3D::SetShader((Direct3D::SHADER_TYPE)type);

	//�p�[�c��1���`��
	for (int k = 0; k < parts_.size(); k++)
	{
		// ���̏u�Ԃ̎����̎p���s��𓾂�

		FbxTime time1, time2;
		time1.SetTime(0, 0, 0, frame1, 0, 0, _frameRate);
		time2.SetTime(0, 0, 0, frame2, 0, 0, _frameRate);

		//�X�L���A�j���[�V�����i�{�[���L��j�̏ꍇ
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawBlendedSkinAnime(transform, time1, time2, blendFactor);
		}
	}
}

void Fbx::Draw(Transform& transform, int type, int frame)
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
	Direct3D::SetShader((Direct3D::SHADER_TYPE)type);

	//�p�[�c��1���`��
	for (int k = 0; k < parts_.size(); k++)
	{
		// ���̏u�Ԃ̎����̎p���s��𓾂�
		FbxTime     time;
		time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);


		//�X�L���A�j���[�V�����i�{�[���L��j�̏ꍇ
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawSkinAnime(transform, time);
		}

		//���b�V���A�j���[�V�����̏ꍇ
		else
		{
			parts_[k]->DrawMeshAnime(transform, time, pFbxScene_);
		}
	}
}


//���C�L���X�g�i���C���΂��ē����蔻��j
void Fbx::RayCast(RayCastData * data)
{
	//���ׂẴp�[�c�Ɣ���
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(data);
	}
}