#pragma once
#include "Engine/Transform.h"

class EnemyBase;
class Player;

class EnemyUi
{
	enum PNG_INDEX {
		GAUGE = 0,
		BACK,
		FOUND,
		MAX,
	};

	int hPict_[MAX];
	int gageAlpha_;			//ゲージの透明度
	float height_;			//uiの高さ
	float parcent;			//hpのパーセント
	float foundParcent_;	//FoundのパAlphaーセント
	Transform transform_[2];//HpとHPBack用のTransform
	EnemyBase* pParent_;
	Player* pPlayer_;

	void SetGageAlpha(int value);

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height：Uiの高さ
	void Draw();
	void SetParcent(float f);
	void InitTargetFoundUi();

};

