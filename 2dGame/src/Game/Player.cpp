#include "Player.h"
#include "Engine/Game.h"
#include "Engine/Input.h"
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

void CPlayer::Update(int dt)
{
	HandleInput();
	
	CGame* game = CGame::GetInstance();

	x += vx * dt;
	y += vy * dt;

	//float l, t, r, b;
	//GetBoundingBox(l, t, r, b);
	if (x + PLAYER_BBOX_WIDTH / 2 > game->GetBackBufferWidth())
		x = game->GetBackBufferWidth() - PLAYER_BBOX_WIDTH / 2;
	if (x < PLAYER_BBOX_WIDTH / 2)
		x = PLAYER_BBOX_WIDTH / 2;

	if (y + PLAYER_BBOX_HEIGHT / 2 > game->GetBackBufferHeight())
		y = game->GetBackBufferHeight() - PLAYER_BBOX_HEIGHT / 2;
	if (y < PLAYER_BBOX_HEIGHT / 2)
		y = PLAYER_BBOX_HEIGHT / 2;

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
