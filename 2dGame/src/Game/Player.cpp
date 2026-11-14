#include "Player.h"

#include "Engine/Game.h"
#include "Engine/Input.h"
#include "Engine/Collision.h"

#include "AssetIDs.h"

void CPlayer::HandleInput()
{
	ReadInput();

	if (in.jumpPressed)
		jumpBufferTimer = PLAYER_JUMP_BUFFER_TIME;
	if (in.dashPressed)
		dashBufferTimer = PLAYER_DASH_BUFFER_TIME;
	if (isOnGround)
		coyoteTimer = PLAYER_COYOTE_TIME;
	if (!isOnGround && isTouchingWall)
		wallCoyoteTimer = PLAYER_WALL_COYOTE_TIME;
	if (isOnGround || isTouchingWall)
		airDashChargeLeft = PLAYER_MAX_AIR_DASHES;

	int moveDirection = in.moveDir;

	if (state == PLAYER_STATE_ATTACK)
		return;
	if (state == PLAYER_STATE_DASH && dashTimer > 0)
		return;

	if (dashBufferTimer > 0 && dashCooldownTimer <= 0 && airDashChargeLeft > 0)
	{
		if (state == PLAYER_STATE_WALL_SLIDE)
			dashDir = -wallDir;
		else
			dashDir = moveDirection != 0 ? moveDirection : nx;
		dashBufferTimer = 0;
		SetState(PLAYER_STATE_DASH);
		return;
	}
	if (state != PLAYER_STATE_WALL_JUMP || wallJumpTimer <= 0)
	{
		if (moveDirection != 0)
			nx = moveDirection;
		vx = moveDirection * PLAYER_RUN_SPEED;
	}

	if (jumpBufferTimer > 0 && coyoteTimer > 0)
	{
		coyoteTimer = 0;
		jumpBufferTimer = 0;
		SetState(PLAYER_STATE_JUMP);
		if (!in.jumpHeld)
			vy *= PLAYER_JUMP_CUT;
		return;
	}

	if (jumpBufferTimer > 0 && wallCoyoteTimer > 0 && !isOnGround)
	{
		jumpBufferTimer = 0;
		wallCoyoteTimer = 0;
		wallJumpTimer = PLAYER_WALL_JUMP_LOCK;
		SetState(PLAYER_STATE_WALL_JUMP);
		if (!in.jumpHeld)
			vy *= PLAYER_JUMP_CUT;
		return;
	}

	if (!isOnGround)
	{
		if (in.jumpReleased && vy < 0)
			vy *= PLAYER_JUMP_CUT;
		if (state == PLAYER_STATE_WALL_JUMP && vy < 0)
			return;
		if (vy >= 0 && isTouchingWall && moveDirection != -wallDir)
		{
			SetState(PLAYER_STATE_WALL_SLIDE);
			return;
		}

		if (vy < 0)
			SetState(PLAYER_STATE_JUMP);
		else
			SetState(PLAYER_STATE_FALL);
		return;
	}

	if (in.attackPressed)
	{
		SetState(PLAYER_STATE_ATTACK);
		return;
	}

	if (moveDirection != 0)
	{
		SetState(PLAYER_STATE_RUN);
		return;
	}

	SetState(PLAYER_STATE_IDLE);
}

void CPlayer::ReadInput()
{
	in = PlayerInput();
	if (!inputEnabled)
		return;

	CInput* input = CInput::GetInstance();

	in.moveDir = input->IsHeld(KEY_MOVE_RIGHT) - input->IsHeld(KEY_MOVE_LEFT);
	in.jumpPressed = input->IsPressed(KEY_JUMP);
	in.jumpHeld = input->IsHeld(KEY_JUMP);
	in.jumpReleased = input->IsReleased(KEY_JUMP);
	in.attackPressed = input->IsPressed(KEY_ATTACK);
	in.dashPressed = input->IsPressed(KEY_DASH);
}

CPlayer::CPlayer(float x, float y) : CGameObject(x, y) {}

void CPlayer::Update(float dt, std::vector<CGameObject*>* coObjects)
{
	if (jumpBufferTimer > 0)
		jumpBufferTimer -= dt;
	if (coyoteTimer > 0)
		coyoteTimer -= dt;
	if (wallCoyoteTimer > 0)
		wallCoyoteTimer -= dt;
	if (wallJumpTimer > 0)
		wallJumpTimer -= dt;

	if (dashBufferTimer > 0)
		dashBufferTimer -= dt;
	if (dashCooldownTimer > 0)
		dashCooldownTimer -= dt;
	if (dashTimer > 0)
		dashTimer -= dt;


	CheckSensors(coObjects);
	HandleInput();

	CGame* game = CGame::GetInstance();

	if (state == PLAYER_STATE_DASH)
	{
		vx = dashDir * PLAYER_DASH_SPEED;
		vy = 0;
	}
	else
	{
		vy += PLAYER_GRAVITY * dt;
		if (state == PLAYER_STATE_WALL_SLIDE)
		{
			if (vy > PLAYER_WALL_SLIDE_SPEED)
				vy = PLAYER_WALL_SLIDE_SPEED;
		}
		else
			if (vy > PLAYER_MAX_FALL_SPEED)
				vy = PLAYER_MAX_FALL_SPEED;
	}


	if (state == PLAYER_STATE_WALL_SLIDE)
	{
		if (vy > PLAYER_WALL_SLIDE_SPEED)
			vy = PLAYER_WALL_SLIDE_SPEED;
	}
	else
		if (vy > PLAYER_MAX_FALL_SPEED)
			vy = PLAYER_MAX_FALL_SPEED;

	CCollision::Process(this, dt, coObjects);

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
	animator.Render(x, y, state == PLAYER_STATE_WALL_SLIDE ? wallDir > 0 : nx < 0);
}

void CPlayer::SetState(int newState)
{
	if (newState == state && newState != PLAYER_STATE_WALL_JUMP)
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
	case PLAYER_STATE_WALL_SLIDE:
		animator.Play(ID_ANI_PLAYER_FALL, false);
		break;
	case PLAYER_STATE_WALL_JUMP:
		vy = -PLAYER_JUMP_SPEED;
		vx = -wallDir * PLAYER_WALL_JUMP_SPEED;
		nx = -wallDir;
		animator.Play(ID_ANI_PLAYER_JUMP, true);
		break;
	case PLAYER_STATE_DASH:
		dashTimer = PLAYER_DASH_TIME;
		dashCooldownTimer = PLAYER_DASH_COOLDOWN;
		if (!isOnGround)
			airDashChargeLeft--;
		nx = dashDir;
		vx = dashDir * PLAYER_DASH_SPEED;
		vy = 0;
		coyoteTimer = 0;
		wallCoyoteTimer = 0;
		wallJumpTimer = 0;
		animator.Play(ID_ANI_PLAYER_DASH, true);
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
	}

	if (e->ny > 0)
		vy = 0;

	if (e->nx != 0)
		vx = 0;
}

void CPlayer::CheckSensors(std::vector<CGameObject*>* coObjects)
{
	float pl, pt, pr, pb;
	GetBoundingBox(pl, pt, pr, pb);

	float st = pt + 2;
	float sb = pb - 2;
	bool overlapLeft = false, overlapRight = false, ground = false;
	for (CGameObject* obj : *coObjects)
	{
		if (obj == this || obj->IsDeleted() || !obj->IsBlock())
			continue;

		float wl, wt, wr, wb;
		obj->GetBoundingBox(wl, wt, wr, wb);

		if (CCollision::Overlap(pr, st, pr + PLAYER_SENSOR_SKIN, sb, wl, wt, wr, wb))
			overlapRight = true;
		if (CCollision::Overlap(pl - PLAYER_SENSOR_SKIN, st, pl, sb, wl, wt, wr, wb))
			overlapLeft = true;
		if (CCollision::Overlap(pl + 1, pb, pr - 1, pb + PLAYER_SENSOR_SKIN, wl, wt, wr, wb))
			ground = true;
	}

	if (overlapRight)
	{
		isTouchingWall = true;
		wallDir = 1;
	}
	else if (overlapLeft)
	{
		isTouchingWall = true;
		wallDir = -1;
	}
	else
		isTouchingWall = false;

	isOnGround = ground;
}
