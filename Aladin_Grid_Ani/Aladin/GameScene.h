#pragma once
#include "Camera.h"
#include "Grid.h"
#include "Aladin.h"


static int score;
class CGameScene
{
protected:
	std::vector<CGameObject*> *listObject;
	CCamera *camera = CCamera::GetInstance();
	bool isChangingScene;
	bool canUseKeyboard;
	static CAladin* Aladin;
	D3DXVECTOR3 AladinCheckpoint;
	
public:
	CGameScene();
	~CGameScene();
	virtual void Render() {};
	virtual void Update(DWORD dt);
	virtual void DestroyAll() {};
	
	//2 cai nay dung de chuyen scene
	virtual bool GetIsChangingScene() { return isChangingScene; };
	virtual void SetIsChangingScene(bool status) { isChangingScene = status; }

	//hàm check, lưu vị trí khi Aladin đi qua check point

	void KeyDown(unsigned short int const &key);
	void KeyUp(unsigned short int const &key);
};

