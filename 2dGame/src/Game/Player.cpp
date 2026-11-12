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

	int moveDirection = 0;
	if (input->IsHeld(VK_RIGHT) || input->IsHeld('D'))
		moveDirection += 1;
	if (input->IsHeld(VK_LEFT) || input->IsHeld('A'))
		moveDirection -= 1;
	if (moveDirection != 0)
		nx = moveDirection;
	vx = moveDirection * PLAYER_RUN_SPEED;

	if (input->IsPressed('X'))
	{
		SetState(PLAYER_STATE_ATTACK);
		return;
	}
	if (input->IsPressed('Z') && isOnGround)
	{
		SetState(PLAYER_STATE_JUMP);
		return;
	}

	if (!isOnGround)
	{
		if (input->IsReleased('Z') && vy < 0)
			vy *= PLAYER_JUMP_CUT;
		if (vy < 0)
			SetState(PLAYER_STATE_JUMP);
		else
			SetState(PLAYER_STATE_FALL);
		return;
	}

	if (moveDirection != 0)
	{
		SetState(PLAYER_STATE_RUN);
		return;
	}

	SetState(PLAYER_STATE_IDLE);
}

CPlayer::CPlayer(float x, float y) : CGameObject(x, y) {}

void CPlayer::Update(float dt, std::vector<CGameObject*>* coObjects)
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

void CPlayer::SetState(int newState)
{
	if (newState == state)
		return;

	switch (newState)
	{
	case PLAYER_STATE_ATTACK:
		vx = 0;
		animator.Play(ID_ANI_PLAYER_ATTACK1, true);
		break;
	case PLAYER_STATE_RUN:
		animator.Play(ID_ANI_PLAYER_RUN, false);
		break;
	case PLAYER_STATE_JUMP:
		vy = -PLAYER_JUMP_SPEED;
		animator.Play(ID_ANI_PLAYER_JUMP, true);
		break;
	case PLAYER_STATE_FALL:
		animator.Play(ID_ANI_PLAYER_FALL, false);
		break;
	default:
		animator.Play(ID_ANI_PLAYER_IDLE, false);
		break;
	}

	CGameObject::SetState(newState);
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
