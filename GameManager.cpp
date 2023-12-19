#include "GameManager.h"
#include "EnemyManager.h"
#include "NavigationAI.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "Engine/GameObject.h"
#include "Engine/Global.h"

//デバッグ用
#include "Engine/Input.h"

namespace GameManager {
	EnemyManager* pEnemyManager_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	GameObject* pParent_ = nullptr;

	void GameManager::Initialize(GameObject* parent)
	{
		pEnemyManager_ = new EnemyManager();
		pEnemyManager_->Initialize(parent);

		Stage* pStage_ = Instantiate<Stage>(parent);
		Player* pPlayer_ = Instantiate<Player>(parent);
		pPlayer_->SetPosition(pStage_->GetPlayerStartPos());
		pParent_ = parent;
		Instantiate<CollisionMap>(parent);

		pNavigationAI_ = new NavigationAI(pStage_);
	}

	void GameManager::Update()
	{
		if (Input::IsKey(DIK_TAB)) {
			OutputDebugString("entity : ");
			int count = (int)pEnemyManager_->GetAllEnemy().size();
			OutputDebugStringA(std::to_string(count).c_str());
			OutputDebugString("\n");
		}

		//デバッグ用
		if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); }
		if (Input::IsKeyDown(DIK_J)) { for (int i = 0; i < 50; i++) pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); }

		if (Input::IsKeyDown(DIK_N)) { pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
		if (Input::IsKeyDown(DIK_H)) { for (int i = 0; i < 50; i++) pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
		
		if (Input::IsKeyDown(DIK_K)) { pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); }
		if (Input::IsKeyDown(DIK_L)) { for(int i = 0;i < 50;i++) pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); }

		if (Input::IsKeyDown(DIK_V)) { pEnemyManager_->AllKillEnemy(); }
	}

	void GameManager::Release() {
		pEnemyManager_->Release(); 
		SAFE_DELETE(pEnemyManager_);
		SAFE_DELETE(pNavigationAI_);
	}

	EnemyManager* GetEnemyManager() { return pEnemyManager_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	GameObject* GetParent() { return pParent_; }
}

