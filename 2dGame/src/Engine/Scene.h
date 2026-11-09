#pragma once

class CScene
{
public:
    virtual ~CScene() {}
    virtual void Load() = 0;            // nạp ảnh, tạo object
    virtual void Update(int dt) = 0;
    virtual void Render() = 0;
    virtual void Unload() = 0;          // xoá object của scene
};