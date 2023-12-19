#pragma once

class EnemyManager;
class NavigationAI;
class GameObject;

namespace GameManager
{
	void Initialize(GameObject* parent);
	void Update();
	void Release();

	EnemyManager* GetEnemyManager();
	NavigationAI* GetNavigationAI();
	GameObject* GetParent();
};