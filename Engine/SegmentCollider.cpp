#include "SegmentCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

//�R���X�g���N�^�i�����蔻��̍쐬�j
//�����FbasePos	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
//�����Fsize	�����蔻��̃T�C�Y
SegmentCollider::SegmentCollider(XMFLOAT3 center, XMVECTOR vec)
{
	center_ = center;
	type_ = COLLIDER_SEGMENT;

	float size = XMVectorGetX(XMVector3Length(vec));
	size_ = XMFLOAT3(size, size, size);
	vec_ = XMVector3Normalize(vec);
	
	//�����[�X���͔���g�͕\�����Ȃ�
#ifdef _DEBUG
	//�e�X�g�\���p����g
	hDebugModel_ = Model::Load("DebugCollision/sphereCollider.fbx");
#endif
}

void SegmentCollider::SetVector(XMVECTOR _vec)
{
	float size = XMVectorGetX(XMVector3Length(_vec));
	size_ = XMFLOAT3(size, size, size); 
	vec_ = XMVector3Normalize(_vec);
}

//�ڐG����
//�����Ftarget	����̓����蔻��
//�ߒl�F�ڐG���Ă��true
bool SegmentCollider::IsHit(Collider* target)
{
	if (target->type_ == COLLIDER_BOX)
		return IsHitBoxVsSegment((BoxCollider*)target, this);
	else if (target->type_ == COLLIDER_CIRCLE)
		return IsHitCircleVsSegment((SphereCollider*)target, this);
	else 
		return IsHitSegmentVsSegment((SegmentCollider*)target, this);
}