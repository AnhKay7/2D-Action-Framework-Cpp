#include "Player.h"

#include "Engine/Game.h"
#include "Engine/Input.h"
#include "Engine/Collision.h"

#include "AssetIDs.h"

void CPlayer::HandleInput()
{
	if (state == PLAYER_STATE_ATTACK)
		return;

	CInput* input = CInput::GetInstance();
	if (input->IsPressed('X'))
	{
		SetState(PLAYER_STATE_ATTACK);
		return;
	}

	int moveDirection = 0;
	if (input->IsHeld(VK_RIGHT) || input->IsHeld('D'))
		moveDirection += 1;
	if (input->IsHeld(VK_LEFT) || input->IsHeld('A'))
		moveDirection -= 1;

	if (moveDirection != 0)
	{
		nx = moveDirection;
		SetState(PLAYER_STATE_RUN);
		return;
	}

	SetState(PLAYER_STATE_IDLE);
}

CPlayer::CPlayer(float x, float y) : CGameObject(x, y) {}

void CPlayer::Update(int dt, std::vector<CGameObject*>* coObjects)
{
	HandleInput();
	
	CGame* game = CGame::GetInstance();

	vy += PLAYER_GRAVITY * dt;
	if (vy > PLAYER_MAX_FALL_SPEED)
		vy = PLAYER_MAX_FALL_SPEED;

	isOnGround = false;
	CCollision::Process(this, dt, coObjects);

	//float l, t, r, b;
	//GetBoundingBox(l, t, r, b);
	if (x + PLAYER_BBOX_WIDTH / 2 > game->GetBackBufferWidth())
		x = game->GetBackBufferWidth() - PLAYER_BBOX_WIDTH / 2;
	if (x < PLAYER_BBOX_WIDTH / 2)
		x = PLAYER_BBOX_WIDTH / 2;

	if (y > game->GetBackBufferHeight() + 50) 
	{
		SetPosition(160, 40);
		vy = 0;
	}

	animator.Update(dt);

	if (animator.IsFinished() && state == PLAYER_STATE_ATTACK)
		SetState(PLAYER_STATE_IDLE);
}

void CPlayer::Render()
{
	animator.Render(x, y, nx < 0);
}

void CPlayer::SetState(int state)
{
	switch (state)
	{
	case PLAYER_STATE_ATTACK:
		vx = 0;
		animator.Play(ID_ANI_PLAYER_ATTACK1, true);
		break;
	case PLAYER_STATE_RUN:
		vx = nx * PLAYER_RUN_SPEED;
		animator.Play(ID_ANI_PLAYER_RUN, false);
		break;
	default:
		vx = 0;
		animator.Play(ID_ANI_PLAYER_IDLE, false);
		break;
	}

	CGameObject::SetState(state);
}

void CPlayer::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PLAYER_BBOX_WIDTH / 2;
	r = x + PLAYER_BBOX_WIDTH / 2;
	
	b = y + PLAYER_FEET_OFFSET;
	t = b - PLAYER_BBOX_HEIGHT;
}

void CPlayer::OnCollisionWith(CCollisionEvent* e)
{
	if (e->ny < 0)
	{
		vy = 0;
		isOnGround = true;
	}

	if (e->ny > 0)
		vy = 0;

	if (e->nx != 0)
		vx = 0;
}
