#include "Animator.h"
#include "Animations.h"
#include "Sprites.h"

void CAnimator::Play(int aniId, bool restart)
{
	if (this->aniId == aniId && !restart)
		return;
	this->aniId = aniId;
	frameIndex = 0;
	frameTimer = 0;
	finished = false;
}

void CAnimator::Update(float dt)
{

	if (aniId == -1)
		return;
	CAnimation* aniPlaying = CAnimations::GetInstance()->Get(aniId);


	if (finished)
		return;
	if (aniPlaying == NULL)
		return;

	int frameCount = aniPlaying->GetFrameCount();
	if (frameCount == 0)
		return;
	frameTimer += dt;
	while (true) {
		CAnimationFrame aniFrame = aniPlaying->GetFrame(frameIndex);
		if (aniFrame.duration <= 0)
			break;
		if (frameTimer >= aniFrame.duration) {
			frameTimer -= aniFrame.duration;
			frameIndex++;
			if (frameIndex >= frameCount) {
				if (aniPlaying->IsLoop())
					frameIndex = 0;
				else
				{
					frameIndex = frameCount - 1;
					finished = true;
					frameTimer = 0;
					return;
				}
			}
		}
		else break;
	}
}

void CAnimator::Render(float x, float y, bool flipX)
{
	if (aniId == -1)
		return;
	CAnimation* aniPlaying = CAnimations::GetInstance()->Get(aniId);

	if (aniPlaying == NULL)
		return;
	if (aniPlaying->GetFrameCount() == 0) 
		return;

	CAnimationFrame aniFrame = aniPlaying->GetFrame(frameIndex);
	int spriteId = aniFrame.spriteId;

	CSprite* sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
		return;

	sprite->Draw(x, y, flipX);
}