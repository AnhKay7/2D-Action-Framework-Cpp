#pragma once
#include <vector>

struct CAnimationFrame
{
    int spriteId;    
    int duration;    
};

class CAnimation
{
private:
    std::vector<CAnimationFrame> frames;   
    bool loop;
public:
    CAnimation(bool loop = true) { this->loop = loop; }

    void AddFrame(int spriteId, int duration);
    int  GetFrameCount() { return (int)frames.size(); }
    const CAnimationFrame& GetFrame(int index) { return frames[index]; }
    bool IsLoop() { return loop; }
};