#include "Player.h"
#include <cmath>
#include "Engine/Game.h"
#include "Engine/Input.h"
#include "Engine/Collision.h"
#include "Game/AssetIDs.h"    

using namespace PlayerStats;

static float Approach(float current, float target, float step)
{
	if (current < target) return min(current + step, target);
	if (current > target) return max(current - step, target);
	return current;
}

static int Sign(float v) { return (v > 0) - (v < 0); }

void CPlayer::ReadInput()
{
	in = PlayerInput();
	if (!inputEnabled)
		return;

	CInput* input = CInput::GetInstance();
	in.moveDir = input->IsHeld(KEY_MOVE_RIGHT) - input->IsHeld(KEY_MOVE_LEFT);
	in.jumpPressed = input->IsPressed(KEY_JUMP);
	in.jumpHeld = input->IsHeld(KEY_JUMP);
	in.runHeld = input->IsHeld(KEY_RUN);
}

void CPlayer::Update(float dt, std::vector<CGameObject*>* coObjects)
{
	if (coyoteTimer > 0)     coyoteTimer -= dt;
	if (jumpBufferTimer > 0) jumpBufferTimer -= dt;

	CheckSensors(coObjects);
	ReadInput();

	UpdateHorizontal(dt);
	UpdateVertical(dt);

	CCollision::Process(this, dt, coObjects);

	UpdateMeter(dt);
	UpdateState();

	if (y > fallLimitY)
	{
		SetPosition(spawnX, spawnY);
		vx = vy = 0;
		meter = 0;
	}

	animator.Update(dt);
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
	case PLAYER_STATE_WALK: animator.Play(ID_ANI_PLAYER_WALK, false); break;
	case PLAYER_STATE_RUN:  animator.Play(ID_ANI_PLAYER_RUN, false); break;
	case PLAYER_STATE_SKID: animator.Play(ID_ANI_PLAYER_SKID, false); break;
	case PLAYER_STATE_JUMP: animator.Play(ID_ANI_PLAYER_JUMP, false); break;
	case PLAYER_STATE_FALL: animator.Play(ID_ANI_PLAYER_FALL, false); break;
	default:                animator.Play(ID_ANI_PLAYER_IDLE, false); break;
	}

	CGameObject::SetState(newState);
}

void CPlayer::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BBOX_W / 2;
	r = x + BBOX_W / 2;
	b = y + FEET_OFFSET;
	t = b - BBOX_H;
}

void CPlayer::OnCollisionWith(CCollisionEvent* e)
{
	if (e->ny != 0) vy = 0;     // chạm sàn hoặc cụng trần
	if (e->nx != 0) vx = 0;     // đâm tường
}

void CPlayer::CheckSensors(std::vector<CGameObject*>* coObjects)
{
	float pl, pt, pr, pb;
	GetBoundingBox(pl, pt, pr, pb);

	bool ground = false;
	for (CGameObject* obj : *coObjects)
	{
		if (obj == this || obj->IsDeleted() || !obj->IsBlock())
			continue;
		float ol, ot, orr, ob;
		obj->GetBoundingBox(ol, ot, orr, ob);
		if (CCollision::Overlap(pl + 1, pb, pr - 1, pb + SENSOR_SKIN, ol, ot, orr, ob))
			ground = true;
	}
	isOnGround = ground;
}

void CPlayer::UpdateHorizontal(float dt)
{
	const bool onGround = IsOnGround();
	isSkidding = false;

	if (in.moveDir == 0)
	{
		if (!onGround)
			return; 

		vx = Approach(vx, 0.0f, FRICTION * dt);
		return;
	}

	float maxSpeed = WALK_MAX;
	if (in.runHeld)
	{
		maxSpeed = IsMeterFull() ?TOP_MAX : RUN_MAX;
	}

	nx = in.moveDir;

	const float target = in.moveDir * maxSpeed;
	const bool isReversing = (vx != 0.0f && Sign(vx) != in.moveDir);
	float acceleration = PlayerStats::ACCEL;

	if (isReversing)
	{
		if (onGround)
		{
			acceleration = SKID_DECEL;
			if (fabs(vx) > SKID_MIN_SPEED)
				isSkidding = true;
		}
	}
	else if (fabs(vx) > maxSpeed)
	{
		if (!onGround)
			return;

		acceleration = FRICTION; 
	}

	vx = Approach(vx, target, acceleration * dt);
}

void CPlayer::UpdateVertical(float dt)
{
	if (IsOnGround())
		coyoteTimer = COYOTE_TIME;
	if (in.jumpPressed)
		jumpBufferTimer = JUMP_BUFFER_TIME;

	if (coyoteTimer > 0 && jumpBufferTimer > 0) 
	{
		vy = -JumpSpeedForCurrentRun();
		coyoteTimer = 0;
		jumpBufferTimer = 0;
	}

	if (vy < 0 && in.jumpHeld)
		vy += GRAVITY_HOLD * dt;
	else
		vy += GRAVITY * dt;

	if (vy > MAX_FALL)
		vy = MAX_FALL;
}

void CPlayer::UpdateMeter(float dt)
{	
	if (IsOnGround()) 
	{
		if (in.runHeld && fabs(vx) >= RUN_MAX - 0.001f)
			meter += dt / METER_FILL_TIME;
		else
			meter -= dt / METER_DRAIN_TIME;
	}

	if (meter > METER_MAX)
		meter = METER_MAX;
	if (meter < 0)
		meter = 0;

}

void CPlayer::UpdateState()
{
	if (!IsOnGround() || vy < 0) 
	{
		if (vy < 0)
			SetState(PLAYER_STATE_JUMP);
		else
			SetState(PLAYER_STATE_FALL);
		return;
	}

	if (isSkidding)
	{
		SetState(PLAYER_STATE_SKID);
		return;
	}

	if (vx == 0)
	{
		SetState(PLAYER_STATE_IDLE);
		return;
	}

	if (IsMeterFull() && fabs(vx) >= TOP_MAX - 0.001f)
	{
		SetState(PLAYER_STATE_RUN);
		return;
	}

	SetState(PLAYER_STATE_WALK);
}

float CPlayer::JumpSpeedForCurrentRun()
{
	if (fabs(vx) >= JUMP_FAST_FROM)
		return JUMP_SPEED_FAST;
	if (fabs(vx) >= JUMP_MID_FROM)
		return JUMP_SPEED_MID;
	return JUMP_SPEED_SLOW;
}
