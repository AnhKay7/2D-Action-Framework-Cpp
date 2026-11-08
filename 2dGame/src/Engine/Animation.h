#pragma once
#include <vector>

struct CAnimationFrame
{
    int spriteId;    // thẻ nào
    int duration;    // hiện bao nhiêu ms
};

// Kịch bản: chỉ chứa dữ liệu, không biết "đang chạy tới đâu"
class CAnimation
{
private:
    std::vector<CAnimationFrame> frames;   // giống List<Frame> bên C#
    bool loop;
public:
    CAnimation(bool loop = true) { this->loop = loop; }

    void AddFrame(int spriteId, int duration);
    int  GetFrameCount() { return (int)frames.size(); }
    const CAnimationFrame& GetFrame(int index) { return frames[index]; }
    bool IsLoop() { return loop; }
};