#include "Animations.h"
#include "Debug.h"

CAnimations* CAnimations::instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (instance == NULL)
		instance = new CAnimations();
	return instance;
}

void CAnimations::Add(int id, CAnimation* ani)
{
	if (ani == NULL)
		return;

	if (animations.count(id))
		delete animations[id];

	animations[id] = ani;
}

CAnimation* CAnimations::Get(int id)
{
	auto it = animations.find(id);
	if (it == animations.end())
	{
		DebugOut(L"[ERROR] Animation id %d khong ton tai\n", id);
		return NULL;
	}
	return it->second;
}

void CAnimations::Clear()
{
	for (auto& ani : animations)
	{
		delete ani.second;
	}

	animations.clear();
}
