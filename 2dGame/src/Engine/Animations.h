#pragma once
#include <unordered_map>
#include "Animation.h"

class CAnimations
{
private:
	static CAnimations* instance;
	std::unordered_map<int, CAnimation*> animations;
	CAnimations() {}
public:
	static CAnimations* GetInstance();

	void Add(int id, CAnimation* ani);
	CAnimation* Get(int id);
	void Clear();
};

