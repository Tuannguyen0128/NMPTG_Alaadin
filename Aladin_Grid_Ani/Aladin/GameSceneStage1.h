#pragma once
#include "GameScene.h"
#include "TileMap.h"

class CGameSceneStage1 :public CGameScene
{
	CTileMap *tileMap;
	CGrid * gridGame;
	vector<CGameObject*> listObj;

public:
	CGameSceneStage1();
	~CGameSceneStage1();
	void Update(DWORD dt);
	void Render();
	bool GetIsChangingScene();
	void SetIsChangingScene(bool status);
	void DestroyAll();
	void CheckCollision();
	void CheckCollisionAladinWithGround();
};

