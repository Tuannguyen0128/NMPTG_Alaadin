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
	LPANIMATION ani;
	ani = new CAnimation(100);	// idle right
	ani->Add(101);
	animations->Add(100, ani);

	ani = new CAnimation(100);	// run right
	for (int i = 200; i < 213; i++)
		ani->Add(i);
	animations->Add(101, ani);

	ani = new CAnimation(100);	// idle left
	ani->Add(102);
	animations->Add(102, ani);

	ani = new CAnimation(100);	// run left
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
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER +0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	this->AddAnimation(Aladin_ANI_BASE_NUMBER + 0);
	//this->SetPosition(50.0f, 0);

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
		x = this->x;
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

