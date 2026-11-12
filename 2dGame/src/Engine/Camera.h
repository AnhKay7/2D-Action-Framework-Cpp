#pragma once
class CGameObject;

class CCamera
{
	float x = 0, y = 0;
	float viewW = 320, viewH = 240;
	float left = 0, top = 0, right = 320, bottom = 240;
	CGameObject* target = nullptr;

	void ClampToBounds();
	void Apply();

public:
	void SetViewSize(float w, float h) { viewW = w; viewH = h; }
	void SetBounds(float l, float t, float r, float b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	void Follow(CGameObject* obj) { target = obj; }

	void SnapToTarget();
	void Update(float dt);
};

