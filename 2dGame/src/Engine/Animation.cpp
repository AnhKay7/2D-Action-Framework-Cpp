#include "Animation.h"

void CAnimation::AddFrame(int spriteId, int duration)
{
	frames.push_back({ spriteId, duration });
}
