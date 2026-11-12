#include "Camera.h"
#include "GameObject.h"
#include "Game.h"

#define CAM_DZ_LEFT    140     // target lọt ra ngoài [140, 180] theo ngang thì camera mới chạy
#define CAM_DZ_RIGHT   180
#define CAM_DZ_TOP      80     // ngoài [80, 160] theo dọc thì camera mới chạy
#define CAM_DZ_BOTTOM  160

void CCamera::ClampToBounds()
{
	if (x < left)
		x = left;
	if (x + viewW > right)
		x = right - viewW;

	if (y < top)
		y = top;
	if (y + viewH > bottom)
		y = bottom - viewH;
}

void CCamera::Apply()
{
	CGame::GetInstance()->SetCamPos(x, y);
}

void CCamera::SnapToTarget()
{
	if (target == nullptr)
		return;

	float tx, ty;
	target->GetPosition(tx, ty);

	x = tx - viewW / 2, y = ty - viewH / 2;
	ClampToBounds();
	Apply();
}

void CCamera::Update(float dt)
{
	if (target == nullptr)
		return;

	float tx, ty;
	target->GetPosition(tx, ty);

	float sx = tx - x;
	if (sx < CAM_DZ_LEFT)
		x = tx - CAM_DZ_LEFT;
	if (sx > CAM_DZ_RIGHT)
		x = tx - CAM_DZ_RIGHT;

	float sy = ty - y;
	if (sy < CAM_DZ_TOP)
		y = ty - CAM_DZ_TOP;
	if (sy > CAM_DZ_BOTTOM)
		y = ty - CAM_DZ_BOTTOM;

	ClampToBounds();
	Apply();
}
