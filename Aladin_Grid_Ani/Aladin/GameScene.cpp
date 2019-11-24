#include "GameScene.h"


CAladin* CGameScene::Aladin = NULL;

CGameScene::CGameScene()
{
	Aladin = new CAladin();
	//objects.push_back(Aladin);
}


CGameScene::~CGameScene()
{
}

void CGameScene::Update(DWORD dt)
{
	//Xet de xoa toan bo enemy de load enemy man khac
}

void CGameScene::KeyDown(unsigned short int const &key)
{
	
	switch (key)
	{
		case UP_KEY:
		{
			Aladin->SetState(Aladin_STATE_UP);
			break;
		}
		case DOWN_KEY:
		{
			Aladin->SetState(Aladin_STATE_DOWN);
			break;
		}
		case LEFT_KEY:
		{
			Aladin->SetState(Aladin_STATE_RUN_LEFT);
			break;
		}
		case RIGHT_KEY:
		{
			Aladin->SetState(Aladin_STATE_RUN_RIGHT);
			break;
		}
		case DIK_SPACE:
			Aladin->SetState(Aladin_STATE_JUMP);
			break;
		case A_KEY:
			Aladin->SetState(Aladin_STATE_HIT);
			break;
		case L_KEY:
			Aladin->SetState(Aladin_STATE_ONWALL);
			break;
		case D_KEY:
			Aladin->SetState(Aladin_STATE_ATTACKED);
			
			break;
		default:
			Aladin->SetState(Aladin_STATE_IDLE);
			break;
	}
	
}

void CGameScene::KeyUp(unsigned short int const &key)
{
	switch (key)
	{
	case UP_KEY:
	{
		//Aladin->SetIsUp(false);
		break;
	}
	case DOWN_KEY:
	{
//		Aladin->SetIsDown(false);
		break;
	}
	case LEFT_KEY:
	{
		Aladin->SetState(Aladin_STATE_RUN_LEFT);
		break;
	}
	case RIGHT_KEY:
	{
		Aladin->SetState(Aladin_STATE_RUN_RIGHT);
		break;
	}
	default:
		Aladin->SetState(Aladin_STATE_IDLE);
		break;
	}
}
