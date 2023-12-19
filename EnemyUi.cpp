#include "EnemyUi.h"
#include "EnemyBase.h"
#include "Engine/Camera.h"
#include "Engine/Image.h"
#include "Player.h"
#include "Engine/Direct3D.h"

namespace {
	const float maxLength = 50.0f;
	const float defSizeX = 2.0f;
	const float defSizeY = 0.5f;
	const float drawSize = 0.85f;
	const int maxAlpha = 255;
	const int alphaValue = 30;
	const float PngSizeX = 256.0f;
	float halfSize = 0.0f;
}

float EeaseIn(float i);

//‚±‚êˆ—Œ¸‚ç‚¹‚é‚Ì‚Å‚Í
void EnemyUi::SetGageAlpha(int value)
{
	gageAlpha_ += value;
	if (gageAlpha_ > maxAlpha) {
		gageAlpha_ = maxAlpha;
	}
	else if (gageAlpha_ < 0) {
		gageAlpha_ = 0;
	}
	Image::SetAlpha(hPict_[0], gageAlpha_);
	Image::SetAlpha(hPict_[1], gageAlpha_);
}

EnemyUi::EnemyUi(EnemyBase* parent)
	: pParent_(parent), pPlayer_(nullptr), hPict_{ -1, -1, -1 }, parcent(1.0f), height_(0.0f), gageAlpha_(0), foundParcent_(0.0f)
{
}

EnemyUi::~EnemyUi()
{
}

void EnemyUi::Initialize(float height)
{
    height_ = height;
	pPlayer_ = (Player*)pParent_->FindObject("Player");

	std::string fileName[] = { "Gauge", "GaugeFrame", "TargetFound" };
	for (int i = 0; i < MAX; i++) {
		hPict_[i] = Image::Load("Png/" + fileName[i] + ".png");
		assert(hPict_[i] >= 0);
	}

	halfSize = PngSizeX / (float)Direct3D::screenWidth_;

	transform_[0].scale_.x = defSizeX;
	transform_[0].scale_.y = defSizeY;
	transform_[1] = transform_[0];

}

void EnemyUi::Draw()
{
	XMFLOAT3 pos = pParent_->GetPosition();
	pos.y += height_; 
	
	XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&pos), Camera::GetViewMatrix());
	v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
	float x = XMVectorGetX(v2);
	float y = XMVectorGetY(v2);

	if (foundParcent_ > 0.0f) {
		foundParcent_ -= 0.01f;
		int alpha = (int)(EeaseIn(foundParcent_) * (float)maxAlpha);
		Transform foundTrans;
		foundTrans.position_ = XMFLOAT3(x, y + 0.05f, 0.0f);
		Image::SetAlpha(hPict_[FOUND], alpha);
		Image::SetTransform(hPict_[FOUND], foundTrans);
		Image::Draw(hPict_[FOUND]);
	}

	if (parcent >= 1.0f) {
		return;
	}

	//‰æŠp§ŒÀ‚·‚é
	if (x >= drawSize || y >= drawSize || x <= -drawSize || y <= -drawSize || XMVectorGetZ(v2) > 1.0f) {
		SetGageAlpha(-alphaValue);
	}
	else {
		XMFLOAT3 fCamPos = pPlayer_->GetPosition();
		XMVECTOR vCamPos = XMLoadFloat3(&fCamPos);
		XMVECTOR vPos = XMLoadFloat3(&pos);
		float length = XMVectorGetX(XMVector3Length(vCamPos - vPos));

		//”ÍˆÍ“à‚©‚Ç‚¤‚©
		if (length < maxLength) {
			SetGageAlpha(alphaValue);
		}
		else {
			SetGageAlpha(-alphaValue);
		}
	}

	//“§–¾“x‚PˆÈã‚È‚çHPGauge•\Ž¦
	if(gageAlpha_ > 0)
	for (int i = 0; i < 2; i++) {
		transform_[i].position_.x = x - halfSize;
		transform_[i].position_.y = y;
		Image::SetTransform(hPict_[i], transform_[i]);
		Image::Draw(hPict_[i]);
	}

}

void EnemyUi::SetParcent(float f)
{
	parcent = f;
	transform_[0].scale_.x = parcent * defSizeX;
}

void EnemyUi::InitTargetFoundUi()
{
	foundParcent_ = 1.0f;
}

float EeaseIn(float i)
{
	return i * i * i * i * i;
}
