#pragma once


class CAnimator
{
private:
    int aniId = -1;          
    int frameIndex = 0;      
    float frameTimer = 0;
    bool finished = false;   

public:
    void Play(int aniId, bool restart = false); 
    void Update(float dt);
    void Render(float x, float y, bool flipX = false);
    bool IsFinished() { return finished; }
    int  GetFrameIndex() { return frameIndex; }
};
