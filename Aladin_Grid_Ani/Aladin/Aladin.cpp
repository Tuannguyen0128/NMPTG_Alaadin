#include "Aladin.h"
#include "debug.h"
//#include "Game.h"
#include <algorithm>
#include "Textures.h"



void CAladin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	preY = y;
	if (x < 0)
		x = 0;
	if (x > 2220)
		x= 2220;
	y += dy;
	if(!(isHit!=0 && !isJump && !isSit) && !isOnWall)
		x += dx;

	if (onGround)//chạm đất
	{
		
		//isOnWall = false;
		canJump = true;
		//if(preY>y) //Neu bay xuong thi y=0
			//vy = 0;
		if (isJump != -1)
			isJump = 0;
		//DebugOut(L"Aladin Cham dat\n");
		
	}
	else
	{
		// Simple fall down
		if(!isOnWall)
			vy -= Aladin_GRAVITY*dt;
		//DebugOut(L"Aladin Khong Cham dat\n");
	}
	if (this->y < 8) //xet cho khoi rot gay bug thoi 9
	{
		//this->y = 8;
		this->SetPosition(30, 100);
		vy = 0; dy = 0;
	}
	
	//DebugOut(L"%f \n", vy);
}
#pragma region Render


void CAladin::Render()
{

	int ani = Aladin_ANI_IDLE_RIGHT;
	//Xét các trạng thái cơ bản
	//		+ các trạng thái đi kèm nằm trong trong cơ bản: Hit
	// Jump: (JumpHit, JumpSpecialHit=Jumpthrow), Jump
	// Sit: (SitHit,SitSpecialHit=SitThrow),Sit
	// Climb: (ClimbIdle,ClimbMove)
	// Attacked: 
	// Còn lại là trạng thái đứng bình thường dưới đất:
	//
	//
	//Nếu đang nhảy thì render trạng thái đang nhảy
	if (isJump)//jump
	{
		Jump(ani);
	}
	else if (isSit)//Nếu đang ngồi thì render trạng thái đang ngồi
	{
		Sit(ani);
	}
	else if (isOnWall)//Nếu đang leo thì render 
	{
		OnWall(ani);
	}
	else//còn lại render trạng thái đứng
	{
		Stand(ani);
	}

	int alpha = 255;

	D3DXVECTOR3 pos;
	pos.x = this->x;
	pos.y = this->y;
	pos.z = 0;
	pos = camera->SetPositionInViewPort(pos); //Nhân lại thành tọa độ trong viewport
											  //Canh số lại để vẽ vì đứa cắt hình bị lệnh
	if (ani == Aladin_ANI_ATTACK_RIGHT || ani == Aladin_ANI_SITATTACK_RIGHT)
		pos.x += 12;
	else if (ani == Aladin_ANI_ATTACK_LEFT || ani == Aladin_ANI_SITATTACK_LEFT)
		pos.x -= 12;
	if (isSit)
		pos.y += 5;

	if (attacked != 0) //Nếu bị đánh thì render nhấp nháy
	{
		Attacked(ani);
		if (attacked % 2 == 0)
		{
			animations[ani]->Render(pos.x, pos.y, alpha);
		}
		else
			animations[ani]->Render(pos.x, pos.y, alpha - 100);
	}
	else //còn không bị đánh thì bình thường
		animations[ani]->Render(pos.x, pos.y, alpha);
	//DebugOut(L"%d\n", attacked);
	this->RenderBoundingBox();
}

//Render state

void CAladin::Jump(int & ani)
{
	//Jump and Hit
	if (isHit != 0)
	{
		if (isUp)//Skill đặc biệt
		{
			if (nx > 0)
				ani = Aladin_ANI_THROW_RIGHT;
			else
				ani = Aladin_ANI_THROW_LEFT;

		}
		else//Danh binh thuong
		{
			if (nx < 0)
				ani = Aladin_ANI_ATTACK_LEFT;
			else
				ani = Aladin_ANI_ATTACK_RIGHT;
		}
		isHit++;
		if (isHit >= 8)
			isHit = 0;
	}
	//Nếu k đánh thì nhảy thôi
	else
	{
		if (nx >= 0)
			ani = Aladin_ANI_JUMP_RIGHT;
		else
			ani = Aladin_ANI_JUMP_LEFT;
	}
}

void CAladin::Sit(int &ani)
{
	if (isHit)//Ngoi danh
	{
		if (nx<0)
			ani = Aladin_ANI_SITATTACK_LEFT;
		else
			ani = Aladin_ANI_SITATTACK_RIGHT;
		isHit++;
		if (isHit >= 8)
			isHit = 0;
	}
	else//Ngoi binh thuong
	{
		if (nx < 0)
			ani = Aladin_ANI_SIT_LEFT;
		else
			ani = Aladin_ANI_SIT_RIGHT;
	}
}

void CAladin::OnWall(int & ani)
{
	vy = 0;
	if (canClimbUpDown)//Nếu được leo
	{
		if (isUp)
		{
			vy = Aladin_CLIMB_SPEED;
			if (nx > 0)
				ani = Aladin_ANI_CLIMB_RIGHT;
			else
				ani = Aladin_ANI_CLIMB_LEFT;
		}
		else if (isDown)
		{
			vy = -Aladin_CLIMB_SPEED;
			if (nx > 0)
				ani = Aladin_ANI_CLIMB_RIGHT;
			else
				ani = Aladin_ANI_CLIMB_LEFT;
		}
		else //đứng yên trên tường
		{
			if (nx > 0)
				ani = Aladin_ANI_ONWALL_RIGHT;
			else
				ani = Aladin_ANI_ONWALL_LEFT;
		}
		//DebugOut(L"%d \n", isDown);

	}
	else //đứng yên trên tường
	{
		if (nx > 0)
			ani = Aladin_ANI_ONWALL_RIGHT;
		else
			ani = Aladin_ANI_ONWALL_LEFT;
	}
}

void CAladin::Stand(int & ani)
{
	if (isHit)//Nếu đánh
	{
		if (isUp)//Skill đặc biệt
		{
			if (nx > 0)
				ani = Aladin_ANI_THROW_RIGHT;
			else
				ani = Aladin_ANI_THROW_LEFT;
		}
		else//Danh binh thuong
		{
			if (nx < 0)
				ani = Aladin_ANI_ATTACK_LEFT;
			else
				ani = Aladin_ANI_ATTACK_RIGHT;
		}
		isHit++;
		if (isHit >= 8)
		{
			isHit = 0;
		}
		vx = 0;
	}
	else if (vx != 0)
	{

		if (nx > 0)//Render ben phai
		{
			ani = Aladin_ANI_RUN_RIGHT;
		}
		else//Render ben trai
		{
			ani = Aladin_ANI_RUN_LEFT;
		}
	}
	else
	{
		if (nx > 0)//Render ben phai
		{
			ani = Aladin_ANI_IDLE_RIGHT;
		}
		else//Render ben trai
		{
			ani = Aladin_ANI_IDLE_LEFT;
		}
	}
}

void CAladin::Attacked(int & ani)
{
	if (!isOnWall)
	{
		if (nx < 0)
		{
			this->x += 3;
			ani = Aladin_ANI_ATTACKED_LEFT;
		}

		else
		{
			this->x -= 3;
			ani = Aladin_ANI_ATTACKED_RIGHT;
		}
	}
	else
	{
		if (nx < 0)
		{
			ani = Aladin_ANI_ONWALL_LEFT;
		}
		else
		{
			ani = Aladin_ANI_ONWALL_RIGHT;
		}

	}
	attacked++;
	if (attacked >= 10)
		attacked = 0;
}

#pragma endregion

#pragma region State
//Hàm để set trạng thái khi nhấn phím thôi. Hàm render mới chính
void CAladin::SetState(int state)
{
	CGameObject::SetState(state);

	//isUp = false;
	switch (state)
	{
	case Aladin_STATE_IDLE:
		vx = 0;
		dy = 0;
		isSit = false;
		isUp = false;
		isDown = false;
		break;
	case Aladin_STATE_RUN_RIGHT:
		vx = Aladin_SPEED;
		if(!isOnWall)
			nx = 1;
		isSit = false;
		isUp = false;
		isDown = false;
		break;
	case Aladin_STATE_RUN_LEFT:
		vx = -Aladin_SPEED;
		if (!isOnWall)
			nx = -1;
		isSit = false;
		isUp = false;
		isDown = false;
		break;
	case Aladin_STATE_JUMP:
		if (canJump)
		{
			vy = +AladinJumpForce;
		}
		onGround = false;
		isJump = 1;
		canJump = false;
		isSit = false;
		canClimbUpDown = false;
		isOnWall = false;
		break;
	case Aladin_STATE_HIT:
		isHit = 1;
		vx = 0;
		break;
	case Aladin_STATE_DOWN:
		isDown = true;
		if(isJump==0&&!isOnWall)
			isSit = 1;
		vx = 0;
		isUp = false;
		if (isOnWall&&onGround)
			isOnWall = false;
		break;
	case Aladin_STATE_UP:
		isUp = true;
		isDown = false;

		break;
	case Aladin_STATE_ONWALL:
		canClimbUpDown = true;
		isOnWall = true;
		break;
	case Aladin_STATE_ATTACKED:
		attacked = 1;
		break;
	}
}
#pragma endregion

#pragma region Load resource

void CAladin::LoadResource()
{
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_Aladin, L"textures\\Aladin.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texAladin = textures->Get(ID_TEX_Aladin);

	D3DXVECTOR2 zero = D3DXVECTOR2(0, 0);
	D3DXVECTOR2 scaleNguoc = D3DXVECTOR2(-1, 1);

	sprites->Add(101, 0, 5, 40, 60, texAladin, zero, zero, zero, 0);//idle
	sprites->Add(102, 0, 5, 40, 60, texAladin, zero, zero, scaleNguoc, 0);
#pragma region climb
	//climp up
	sprites->Add(120, 10, 1362, 40, 1450, texAladin, zero, zero, zero, 0);
	sprites->Add(121, 50, 1362, 82, 1450, texAladin, zero, zero, zero, 0);
	sprites->Add(122,93, 1370,123, 1450, texAladin, zero, zero, zero, 0);
	sprites->Add(123,132, 1376, 175, 1440, texAladin, zero, zero, zero, 0);
	sprites->Add(124,185, 1362, 217, 1440, texAladin, zero, zero, zero, 0);
	sprites->Add(125, 230, 1357,2602, 1445, texAladin, zero, zero, zero, 0);
	sprites->Add(126, 274, 1366, 274+28, 1366+79, texAladin, zero, zero, zero, 0);
	sprites->Add(127, 314, 1377, 314+32, 1377+63, texAladin, zero, zero, zero, 0);
	sprites->Add(128, 356, 1377, 356+42, 1377+62, texAladin, zero, zero, zero, 0);
	sprites->Add(129, 406, 1362, 406+28, 1362+81, texAladin, zero, zero, zero, 0);
	///climp down 129-120
	//sprites->Add(, , , , , texAladin, zero, zero, zero, 0);

	//climp left
	sprites->Add(140,10 ,1467 ,10+40 ,1467+78 , texAladin, zero, zero, zero, 0);
	sprites->Add(141,57 ,1469 ,57+54 ,1469+72 , texAladin, zero, zero, zero, 0);
	sprites->Add(142,120 ,1470 ,120+52 ,1470+77 , texAladin, zero, zero, zero, 0);
	sprites->Add(143,191 ,1470 ,191+44 ,1470+80 , texAladin, zero, zero, zero, 0);
	sprites->Add(144,258,1468  ,258+36 ,1468+83 , texAladin, zero, zero, zero, 0);
	sprites->Add(145,311 ,1468 ,311+43 ,1468+83 , texAladin, zero, zero, zero, 0);
	sprites->Add(146,366 ,1471 ,366+50 ,1471+77 , texAladin, zero, zero, zero, 0);
	sprites->Add(147,430 ,1473 ,430+57 ,1473+80 , texAladin, zero, zero, zero, 0);
	sprites->Add(148,517 ,1471 ,517+47 ,1471+81 , texAladin, zero, zero, zero, 0);
	sprites->Add(149,586 ,1473 ,586+34 ,1473+78 , texAladin, zero, zero, zero, 0);
	//climb right
	sprites->Add(150, 10, 1467, 10 + 40, 1467 + 78, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(151, 57, 1469, 57 + 54, 1469 + 72, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(152, 120, 1470, 120 + 52, 1470 + 77, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(153, 191, 1470, 191 + 44, 1470 + 80, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(154, 258, 1468, 258 + 36, 1468 + 83, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(155, 311, 1468, 311 + 43, 1468 + 83, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(156, 366, 1471, 366 + 50, 1471 + 77, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(157, 430, 1473, 430 + 57, 1473 + 80, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(158, 517, 1471, 517 + 47, 1471 + 81, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(159, 586, 1473, 586 + 34, 1473 + 78, texAladin, zero, zero, scaleNguoc, 0);
	
	//climb idle
	sprites->Add(160,7 ,1564 ,7+49 ,1564+88 , texAladin, zero, zero, zero, 0);
	sprites->Add(161,65 ,1564 ,65+47 ,1564+87 , texAladin, zero, zero, zero, 0);
	sprites->Add(162,132 ,1564 ,132+41 ,1564+88 , texAladin, zero, zero, zero, 0);
	sprites->Add(163,181 ,1563 ,181+40 ,1563+88 , texAladin, zero, zero, zero, 0);
	sprites->Add(164, 236, 1562, 236+38,1562+88 , texAladin, zero, zero, zero, 0);
	//climb idle left
	sprites->Add(165, 7, 1564, 7 + 49, 1564 + 88, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(166, 65, 1564, 65 + 47, 1564 + 87, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(167, 132, 1564, 132 + 41, 1564 + 88, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(168, 181, 1563, 181 + 40, 1563 + 88, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(169, 236, 1562, 236 + 38, 1562 + 88, texAladin, zero, zero, scaleNguoc, 0);
	//climb thow aple --left
	sprites->Add(170,13 ,1666 ,13+37 ,1666+87 , texAladin, zero, zero, zero, 0);
	sprites->Add(171,65 ,1666 ,65+41 ,1666+87 , texAladin, zero, zero, zero, 0);
	sprites->Add(172,125 ,1665 ,125+51 ,1665+88 , texAladin, zero, zero, zero, 0);
	sprites->Add(173,192 ,1664 ,192+31 ,1664+89 , texAladin, zero, zero, zero, 0);
	sprites->Add(174,236 ,1664 ,236+57 ,1664+89 , texAladin, zero, zero, zero, 0);
	//climb thow aple right
	sprites->Add(175, 13, 1666, 13 + 37, 1666 + 87, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(176, 65, 1666, 65 + 41, 1666 + 87, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(177, 125, 1665, 125 + 51, 1665 + 88, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(178, 192, 1664, 192 + 31, 1664 + 89, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(179, 236, 1664, 236 + 57, 1664 + 89, texAladin, zero, zero, scaleNguoc, 0);
	//climb attack -left 
	sprites->Add(180,15 ,1769 ,15+48 ,1769+87 , texAladin, zero, zero, zero, 0);
	sprites->Add(181,81 ,1769 ,81+52 ,1769+89 , texAladin, zero, zero, zero, 0);
	sprites->Add(182,146 ,1769 ,146+61 ,1769+93 , texAladin, zero, zero, zero, 0);
	sprites->Add(183,247 ,1773 ,247+53 ,1773+90 , texAladin, zero, zero, zero, 0);
	sprites->Add(184,327 ,1773 ,327+55,1772+89 , texAladin, zero, zero, zero, 0);
	sprites->Add(185, 403, 1775, 403+72, 1775+89, texAladin, zero, zero, zero, 0);
	//climb atack right
	sprites->Add(186, 15, 1769, 15 + 48, 1769 + 87, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(187, 81, 1769, 81 + 52, 1769 + 89, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(188, 146, 1769, 146 + 61, 1769 + 93, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(189, 247, 1773, 247 + 53, 1773 + 90, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(190, 327, 1773, 327 + 55, 1772 + 89, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(191, 403, 1775, 403 + 72, 1775 + 89, texAladin, zero, zero, scaleNguoc, 0);
	//climb jump out 
	sprites->Add(192,10 ,1870 ,10+34 ,1870+73 , texAladin, zero, zero, zero, 0);
	sprites->Add(193,65 ,1873 ,65+25 ,1873+77 , texAladin, zero, zero, zero, 0);
	sprites->Add(194,100 , 1874,100+36 ,1874+71 , texAladin, zero, zero, zero, 0);
	sprites->Add(195, 144,1874 ,144+48 ,1874+64 , texAladin, zero, zero, zero, 0);
	sprites->Add(196, 204,1875 ,204+59 ,1875+ 54 , texAladin, zero, zero, zero, 0);
	sprites->Add(197,281 ,1876 ,281+59 ,1876+54 , texAladin, zero, zero, zero, 0);
	sprites->Add(198,354 ,1869 ,354+52 ,1869+54 , texAladin, zero, zero, zero, 0);
	sprites->Add(199,442 ,1871 ,442+47 , 1871+67, texAladin, zero, zero, zero, 0);
#pragma endregion climb
#pragma region chay-jump
	//sprites->Add(, , , , , texAladin, zero, zero, zero, 0);
	//chạy-phải-hình 16
	sprites->Add(200, 15, 1215, 55, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(201, 65, 1215, 110, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(202, 120, 1215, 160, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(203, 170, 1215, 215, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(204, 220, 1215, 270, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(205, 280, 1215, 325, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(206, 335, 1215, 375, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(207, 385, 1215, 430, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(208, 440, 1215, 475, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(209, 488, 1215, 538, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(210, 545, 1215, 603, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(211, 610, 1215, 665, 1275, texAladin, zero, zero, zero, 0);
	sprites->Add(212, 680, 1215, 725, 1275, texAladin, zero, zero, zero, 0);
	//chạy trái
	sprites->Add(213, 15, 1215, 55, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(214, 65, 1215, 110, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(215, 120, 1215, 160, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(216, 170, 1215, 215, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(217, 220, 1215, 270, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(218, 280, 1215, 325, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(219, 335, 1215, 375, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(220, 385, 1215, 430, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(221, 440, 1215, 475, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(222, 488, 1215, 538, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(223, 545, 1215, 603, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(224, 610, 1215, 665, 1275, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(225, 680, 1215, 725, 1275, texAladin, zero, zero, scaleNguoc, 0);
	////climb -leo cheo 
	//sprites->Add(230, 0, 1465, 50,1545 , texAladin, zero, zero, zero, 0);
	//sprites->Add(231, 55, 1469, 111, 1545, texAladin, zero, zero, zero, 0);
	//sprites->Add(232, 120, 1465, 173, 1545, texAladin, zero, zero, zero, 0);
	////animation là 1 mảng
	//jump phai
	sprites->Add(230, 10, 1153, 62, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(231, 70, 1153, 120, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(232, 125, 1153, 175, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(233, 185, 1153, 230, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(234, 250, 1153, 292, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(235, 305, 1153, 345, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(236, 360, 1153, 402, 1208, texAladin, zero, zero, zero, 0);
	sprites->Add(237, 415, 1153, 461, 1208, texAladin, zero, zero, zero, 0);
	// jump trai
	sprites->Add(238, 10, 1153, 62, 1208, texAladin, zero, zero,   scaleNguoc, 0);
	sprites->Add(239, 70, 1153, 120, 1208, texAladin, zero, zero,  scaleNguoc, 0);
	sprites->Add(240, 125, 1153, 175, 1208, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(241, 185, 1153, 230, 1208, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(242, 250, 1153, 292, 1208, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(243, 305, 1153, 345, 1208, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(244, 360, 1153, 402, 1208, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(245, 415, 1153, 461, 1208, texAladin, zero, zero, scaleNguoc, 0);
#pragma endregion chay-jump
	//sit right
	sprites->Add(250,205 ,500 ,205+42 ,500+49 , texAladin, zero, zero, zero, 0);
	sprites->Add(251,258 ,500 ,258+48 ,500+49 , texAladin, zero, zero, zero, 0);
	sprites->Add(252,315 ,500 ,315+54 ,500+49 , texAladin, zero, zero, zero, 0);
	sprites->Add(253,378 ,500 ,253+53 ,549 , texAladin, zero, zero, zero, 0);
	//sit left
	sprites->Add(254, 205, 500, 205 + 42, 500 + 49, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(255, 258, 500, 258 + 48, 500 + 49, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(256, 315, 500, 315 + 54, 500 + 49, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(257, 378, 500, 253 + 53, 549, texAladin, zero, zero, scaleNguoc, 0);

	//sit -thow apple -right
	sprites->Add(300,9 ,571 ,9+37 ,571+44 , texAladin, zero, zero, zero, 0);
	sprites->Add(301,58 ,567 ,58+37 ,567+48 , texAladin, zero, zero, zero, 0);
	sprites->Add(302, 107,571 ,107+50 ,571+48 , texAladin, zero, zero, zero, 0);
	sprites->Add(303,164 ,578 ,164+82 ,578+37 , texAladin, zero, zero, zero, 0);
	sprites->Add(304,256 ,579 ,256+57 ,579+37 , texAladin, zero, zero, zero, 0);
	//sit -thow apple left
	sprites->Add(305, 9, 571, 9 + 37, 571 + 44, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(306, 58, 567, 58 + 37, 567 + 48, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(307, 107, 571, 107 + 50, 571 + 48, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(307, 164, 578, 164 + 82, 578 + 37, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(309, 256, 579, 256 + 57, 579 + 37, texAladin, zero, zero, scaleNguoc, 0);
	//sprites->Add(, , , , , texAladin, zero, zero, zero, 0);
#pragma region sitattack
	//sit atack -right
	sprites->Add(310,9 ,640 ,47+9 ,33+640 , texAladin, zero, zero, zero, 0);
	sprites->Add(311,64 ,642 ,64+45 ,642+  31 , texAladin, zero, zero, zero, 0);
	sprites->Add(312,115 ,640 ,115+71 ,640+33 , texAladin, zero, zero, zero, 0);
	sprites->Add(313, 193,639 ,92+193 , 639+34, texAladin, zero, zero, zero, 0);
	sprites->Add(314,298 ,640 ,84+298 ,640+33 , texAladin, zero, zero, zero, 0);
	sprites->Add(315,392 ,640 ,392+71 ,640+33 , texAladin, zero, zero, zero, 0);
	sprites->Add(316, 475, 642,475+45 ,642+31 , texAladin, zero, zero, zero, 0);
	//sit attack -left
	sprites->Add(317, 9, 640, 47+9, 33+640, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(318, 64, 642, 64 + 45, 642 + 31, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(319, 115, 640, 115 + 71, 640 + 33, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(320, 193, 639, 92 + 193, 639 + 34, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(321, 298, 640, 84 + 298, 640 + 33, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(322, 392, 640, 392 + 71, 640 + 33, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(323, 475, 642, 475 + 45, 642 + 31, texAladin, zero, zero, scaleNguoc, 0);
#pragma endregion sitattack
#pragma region jumpattack
	//jump --attack right
	sprites->Add(340,13 ,1077 ,13+52 ,1077+54 , texAladin, zero, zero, zero, 0);
	sprites->Add(341,75 ,1075 ,75+50 ,1075+52 , texAladin, zero, zero, zero, 0);
	sprites->Add(342, 140,1067 ,140+47 ,1067+66 , texAladin, zero, zero, zero, 0);
	sprites->Add(343, 199,1078 ,199+52 ,1078+56 , texAladin, zero, zero, zero, 0);
	sprites->Add(344, 266,1079 ,266+83 ,1079+51 , texAladin, zero, zero, zero, 0);
	sprites->Add(345, 361,1087 ,361+47 ,1087+55 , texAladin, zero, zero, zero, 0);
	//jump attack left
	sprites->Add(346, 13, 1077, 13 + 52, 1077 + 54, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(347, 75, 1075, 75 + 50, 1075 + 52, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(348, 140, 1067, 140 + 47, 1067 + 66, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(349, 199, 1078, 199 + 52, 1078 + 56, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(350, 266, 1079, 266 + 83, 1079 + 51, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(351, 361, 1087, 361 + 47, 1087 + 55, texAladin, zero, zero, scaleNguoc, 0);
#pragma endregion jumpattack
#pragma  region jumpthowapple
	//jump thow apple right
	sprites->Add(360,19 ,1005 ,19+49 ,1005+55 , texAladin, zero, zero, zero, 0);
	sprites->Add(361,77 ,1007 ,77+43 ,1007+52 , texAladin, zero, zero, zero, 0);
	sprites->Add(362,132 ,1008 ,132+39 ,1008+53 , texAladin, zero, zero, zero, 0);
	sprites->Add(363,185 ,1008 ,185+52 ,1008+53 , texAladin, zero, zero, zero, 0);
	sprites->Add(364,253 ,1012 ,253+40 ,1012+52 , texAladin, zero, zero, zero, 0); 
	//jump thow apple left
	sprites->Add(365, 19, 1005, 19 + 49, 1005 + 55, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(366, 77, 1007, 77 + 43, 1007 + 52, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(367, 132, 1008, 132 + 39, 1008 + 53, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(368, 185, 1008, 185 + 52, 1008 + 53, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(369, 253, 1012, 253 + 40, 1012 + 52, texAladin, zero, zero, scaleNguoc, 0);
#pragma endregion jumpthowapple
#pragma region attack
	//attack right
	sprites->Add(370,5 , 314,5+45 ,337+ 74, texAladin, zero, zero, zero, 0);
	sprites->Add(371,54 , 314,54+51 ,326+ 74, texAladin, zero, zero, zero, 0);
	sprites->Add(372,115 , 314,115+45 ,314+74 , texAladin, zero, zero, zero, 0);
	sprites->Add(373, 171, 314,171+82 ,324+ 74, texAladin, zero, zero, zero, 0);
	sprites->Add(374, 260, 314,260+51 ,335+ 74, texAladin, zero, zero, zero, 0);
	//attack left
	sprites->Add(375, 5, 314, 5 + 45, 337 + 74, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(376, 54, 314, 54 + 51, 326 + 74, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(377, 115, 314, 115 + 45, 314 + 74, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(378, 171, 314, 171 + 82, 324 + 74, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(379, 260, 314, 260 + 51, 335 + 74, texAladin, zero, zero, scaleNguoc, 0);
	//thow apple right
	sprites->Add(380, 7, 231,7+45 ,231+60 , texAladin, zero, zero, zero, 0);
	sprites->Add(381,57 , 231,57+41 , 231 + 60, texAladin, zero, zero, zero, 0);
	sprites->Add(382,109 , 231,109+40 , 231 + 60, texAladin, zero, zero, zero, 0);
	sprites->Add(383,163 , 231,163+46 , 231 + 60, texAladin, zero, zero, zero, 0);
	sprites->Add(384,221 , 231,221+40 , 231 + 60, texAladin, zero, zero, zero, 0);
	sprites->Add(385,268 , 231, 268+40, 231 + 60, texAladin, zero, zero, zero, 0);
	//thơ apple left
	sprites->Add(386, 7, 231, 7 + 45, 231 + 60, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(387, 57, 231, 57 + 41, 231 + 60, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(388, 109, 231, 109 + 40, 231 + 60, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(389, 163, 231, 163 + 46, 231 + 60, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(390, 221, 231, 221 + 40, 231 + 60, texAladin, zero, zero, scaleNguoc, 0);
	sprites->Add(391, 268, 231, 268 + 40, 231 + 60, texAladin, zero, zero, scaleNguoc, 0);
#pragma endregion attack

	LPANIMATION ani;
#pragma region 1-13
	ani = new CAnimation(100);	// idle right
	ani->Add(101);
	animations->Add(100, ani);

	ani = new CAnimation(80);	// run right
	for (int i = 200; i < 213; i++)
		ani->Add(i);
	animations->Add(101, ani);

	ani = new CAnimation(100);	// idle left
	ani->Add(102);
	animations->Add(102, ani);

	ani = new CAnimation(80);	// run left
	for (int i = 213; i < 226; i++)
		ani->Add(i);
	animations->Add(103, ani);

	//ani = new CAnimation(100);	// climb right
	//ani->Add(120);
	//ani->Add(121);
	//animations->Add(104, ani);

	//ani = new CAnimation(100);	// climb left
	//ani->Add(122);
	//ani->Add(123);
	//animations->Add(105, ani);

	ani = new CAnimation(45);	// jump right
	for (int i = 230; i < 238; i++)
		ani->Add(i);
	animations->Add(106, ani);

	ani = new CAnimation(45);	// jump left
	for (int i = 238; i < 246; i++)
		ani->Add(i);
	animations->Add(107, ani);

	ani = new CAnimation(45);	// attack right
	for (int i = 370; i < 375; i++)
		ani->Add(i);
	animations->Add(108, ani);
	ani = new CAnimation(45);	// attack left
	for (int i = 375; i < 380; i++)
		ani->Add(i);
	animations->Add(109, ani);

	ani = new CAnimation(100);	//jump attack right
	for (int i = 340; i < 346; i++)
		ani->Add(i);
	animations->Add(110, ani);
	ani = new CAnimation(100);	//jump attack left
	for (int i = 346; i < 352; i++)
		ani->Add(i);
	animations->Add(111, ani);
	ani = new CAnimation(45);	//thow apple   right
	for (int i = 380; i < 386; i++)
		ani->Add(i);
	animations->Add(112, ani);
	ani = new CAnimation(45);	//thow apple   left
	for (int i = 386; i < 392; i++)
		ani->Add(i);
	animations->Add(113, ani);
#pragma endregion 1-13
	ani = new CAnimation(45);	//
	for (int i = 386; i < 392; i++)
		ani->Add(i);
	animations->Add(113, ani);
#pragma region old sprite
	//ani = new CAnimation(100);	// attack right
	//							//ani->Add(140);
	//ani->Add(141);
	//ani->Add(142);
	//animations->Add(108, ani);

	//ani = new CAnimation(100);	// attack left
	//							//ani->Add(143);
	//ani->Add(144);
	//ani->Add(145);
	//animations->Add(109, ani);

	//ani = new CAnimation(100);	// jump attack right
	//ani->Add(150);
	//ani->Add(151);
	//ani->Add(152);
	//ani->Add(153);
	//animations->Add(110, ani);

	//ani = new CAnimation(100);	// jump attack left
	//ani->Add(154);
	//ani->Add(155);
	//ani->Add(156);
	//ani->Add(157);
	//animations->Add(111, ani);

	//ani = new CAnimation(70);	// throw right
	//ani->Add(160);
	//ani->Add(161);
	//ani->Add(162);
	//animations->Add(112, ani);

	//ani = new CAnimation(70);	// throw left
	//ani->Add(163);
	//ani->Add(164);
	//ani->Add(165);
	//animations->Add(113, ani);

	//ani = new CAnimation(100);	// sit attack right
	//							//ani->Add(170);
	//							//ani->Add(171);
	//ani->Add(172);
	//ani->Add(173);
	//animations->Add(114, ani);

	//ani = new CAnimation(100);	// sit attack left
	//							//ani->Add(174);
	//							//ani->Add(175);
	//ani->Add(176);
	//ani->Add(177);
	//animations->Add(115, ani);

	//ani = new CAnimation(100);	// sit right
	//ani->Add(170);
	//animations->Add(116, ani);

	//ani = new CAnimation(100);	// sit left
	//ani->Add(174);
	//animations->Add(117, ani);

	//ani = new CAnimation(100);	// on wall right
	//ani->Add(120);
	////ani->Add(121);
	//animations->Add(118, ani);

	//ani = new CAnimation(100);	// on wall left
	//ani->Add(122);
	////ani->Add(123);
	//animations->Add(119, ani);

	//ani = new CAnimation(100);	// attacked right
	//ani->Add(130);
	//animations->Add(120, ani);

	//ani = new CAnimation(100);	// attacked left
	//ani->Add(134);
	//animations->Add(121, ani);
	#pragma endregion old sprite

	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0); // Idle right
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 1);	//ani 1 = run
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 2);	//ani 2 = idle left
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 3);	//ani 3 = run left
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 6);//jump -right
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 7);//jump left
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 8);//attack right
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 9);//attack left
	this->AddAnimation(Aladin_ANI_BASE_NUMBER +10);//jump attack right
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 11);//jump attack left
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 12);//thow aple right
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 13);//thow apple left
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->SetPosition(50.0f, 0);

}


#pragma endregion


#pragma region Collision

void CAladin::GetBoundingBox(float &x, float &y, float &width, float &height)
{
	if (this->isSit)
	{
		x = this->x;
		y = this->y-12;
		width = Aladin_WIDTH_TMP;
		height = Aladin_HEIGHT_TMP-12;
	}
	else if (this->isHit &&!isOnWall/*&& !this->isSit*/)
	{
		x = this->x+25;
		y = this->y - 6;
		width = Aladin_WIDTH_TMP;
		height = Aladin_HEIGHT_TMP - 6;
	}
	else if (this->isJump /*&&!this->isHit*/)
	{
		x = this->x;
		y = this->y - 6;
		width = Aladin_WIDTH_TMP;
		height = Aladin_HEIGHT_TMP - 12;
	}
	else
	{
		x = this->x;
		y = this->y;
		width = Aladin_WIDTH_TMP;
		height = Aladin_HEIGHT_TMP;
	}
	
}
unsigned short int CAladin::isCollitionObjectWithObject(CGameObject * obj)
{
	LPCOLLISIONEVENT e = SweptAABBEx(obj); // kt va chạm giữa 2 object bằng sweptAABB
	if (e->t != 0)
	{
		if (e->t > 0 && e->t <= 1.0f)
		{
			if (e->ny > 0)
			{
				//DebugOut(L"Cham duoi chan Aladin\n");
				SAFE_DELETE(e);
				return OBJ_COLLISION_BOTTOM;
			}
			else if (e->ny < 0)
			{
				//DebugOut(L"Cham dau Aladin\n");
				SAFE_DELETE(e);
				return OBJ_COLLISION_TOP;
			}
			
			if (e->nx > 0)
			{
				//DebugOut(L"Cham ben trai Aladin\n");
				SAFE_DELETE(e);
				return OBJ_COLLISION_LEFT;
			}
			if (e->nx <0)
			{
				//DebugOut(L"Cham ben phai Aladin\n");
				SAFE_DELETE(e);
				return OBJ_COLLISION_RIGHT;
			}
		}
	}
	return checkAABB(obj);
}

#pragma endregion

