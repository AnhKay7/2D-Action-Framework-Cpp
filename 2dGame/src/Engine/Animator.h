#pragma once

// "Nhạc công": mỗi nhân vật giữ một cái riêng
class CAnimator
{
private:
    int aniId = -1;          // đang chơi kịch bản nào (-1 = chưa có)
    int frameIndex = 0;      // đang ở khung thứ mấy
    int frameTimer = 0;      // đã hiện khung này được bao nhiêu ms
    bool finished = false;   // kịch bản không lặp đã chạy hết chưa

public:
    void Play(int aniId, bool restart = false);  // chuyển sang kịch bản khác
    void Update(int dt);                         // cộng thời gian, lật thẻ khi tới lúc
    void Render(float x, float y, bool flipX = false);
    bool IsFinished() { return finished; }
    int  GetFrameIndex() { return frameIndex; }
};
