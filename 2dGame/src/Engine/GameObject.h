#pragma once

class CGameObject
{
protected:                 // protected = lớp con dùng được, bên ngoài thì không
    float x = 0, y = 0;    // vị trí TÂM object, đơn vị pixel
    float vx = 0, vy = 0;  // vận tốc, pixel mỗi ms
    int   nx = 1;          // hướng mặt: 1 = phải, -1 = trái
    int   state = -1;

public:
    CGameObject(float x, float y) { this->x = x; this->y = y; }
    virtual ~CGameObject() {}

    virtual void Update(int dt) = 0;     // = 0: lớp con BẮT BUỘC phải viết
    virtual void Render() = 0;
    virtual void SetState(int state) { this->state = state; }

    // Hộp va chạm (để dành cho giai đoạn 2). Toạ độ màn hình: trái, trên, phải, dưới
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;

    float GetX() { return x; }
    float GetY() { return y; }
    int   GetState() { return state; }
};
