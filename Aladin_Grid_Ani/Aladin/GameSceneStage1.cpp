#include "GameSceneStage1.h"



CGameSceneStage1::CGameSceneStage1() :CGameScene() //gọi lại cái khởi tạo của gamescene để kt con Aladin
{
	if (tileMap == NULL)
		tileMap = new CTileMap(L"ReSource\\Map1-Matrix.txt", L"ReSource\\Map1-Tiles.png");
	if (camera == NULL)
		camera = CCamera::GetInstance();
	if(gridGame==NULL)
		gridGame = new CGrid();
	gridGame->SetFile("ReSource/Map1-Objects.txt");
	gridGame->LoadGrid();
}


CGameSceneStage1::~CGameSceneStage1()
{
	SAFE_DELETE(tileMap);
	SAFE_DELETE(gridGame);
}

void CGameSceneStage1::Update(DWORD dt)
{
	gridGame->GetListObject(listObj, camera); // lấy hết các object "còn Alive" trong vùng camera;
	
	Aladin->Update(dt);
	float AladinX = 0, AladinY = 0;
	Aladin->GetPosition(AladinX, AladinY);

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);  // đã kiểm tra "Alive" lúc lấy từ lưới ra

	if (!Aladin->GetIsDeadth())//chưa chết thì xét va chạm
	{
		CheckCollision();
	}

	//Update lại tọa độ camera
	camera->Update(dt, AladinX, AladinY, tileMap->GetMapWidth(), tileMap->GetMapHeight());

}

void CGameSceneStage1::Render()
{
	tileMap->Render();

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Render();

	Aladin->Render();
}

bool CGameSceneStage1::GetIsChangingScene()
{
	return CGameScene::GetIsChangingScene();;
}

void CGameSceneStage1::SetIsChangingScene(bool status)
{
	CGameScene::SetIsChangingScene(status);
}

void CGameSceneStage1::DestroyAll()
{
	delete(tileMap);
	//delete(ground);
	//delete(listObject);
	delete(camera);
}

void CGameSceneStage1::CheckCollision()
{
	CheckCollisionAladinWithGround();

}

void CGameSceneStage1::CheckCollisionAladinWithGround()
{
	bool grounded = false;
	//DebugOut(L"Obj Size: %d\n", listObj.size());

	for (UINT i = 0; i < listObj.size(); i++)
	{
		if (listObj[i]->GetType() == TYPE_GROUND)
		{
			CGameObject * gameObj = listObj[i];
			unsigned short int collisionCheck = Aladin->isCollitionObjectWithObject(gameObj);
			if (!collisionCheck == OBJ_NO_COLLISION) //Neu co va cham
			{
				float objX, objY, objW, objH;
				gameObj->GetBoundingBox(objX, objY, objW, objH);
				
				if (collisionCheck == OBJ_COLLISION_BOTTOM) // có va chạm xảy ra với nền đất
				{
					grounded = true;//da cham dat
					if (Aladin->y - Aladin_HEIGHT_TMP>objY - 15) //cham 1 it o tren moi tinh
						Aladin->SetPositionY(objY + Aladin_HEIGHT_TMP);
					//Aladin->SetSpeedY(0);
				}

				//else if (collisionCheck == OBJ_COLLISION_LEFT)
				//{
				//	//Aladin->SetSpeedX(0);
				//	//Aladin->SetPositionX(objX +objW);
				//}
				//else if (collisionCheck == OBJ_COLLISION_RIGHT)
				//{
				//	//Aladin->SetPositionX(objX - Aladin_WIDTH_TMP);
				//	//Aladin->SetSpeedX(0);
				//}
				//else if (collisionCheck == OBJ_COLLISION_TOP){}
			}
		}
	}
	Aladin->SetOnGround(grounded);

}



