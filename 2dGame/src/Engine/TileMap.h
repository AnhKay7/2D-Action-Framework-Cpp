#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// Một tile layer đọc từ Tiled
struct TileLayer
{
    std::string name;
    bool visible = true;
    std::vector<int> data;      // width * height ô, 0 = trống, đã bỏ cờ lật
};

// Một object trong object layer của Tiled
struct MapObject
{
    std::string name;
    std::string type;           // ô "Class" trong Tiled
    float x = 0, y = 0;         // góc TRÊN-TRÁI, đơn vị px
    float width = 0, height = 0;
    std::unordered_map<std::string, std::string> props;   // thuộc tính tự thêm, đều lưu dạng chuỗi

    std::string GetString(const std::string& key, const std::string& def = "") const;
    int         GetInt(const std::string& key, int def = 0) const;
    float       GetFloat(const std::string& key, float def = 0) const;
    bool        GetBool(const std::string& key, bool def = false) const;
};

class CTileMap
{
    int width = 0, height = 0;          // số ô
    int tileW = 16, tileH = 16;         // px mỗi ô
    int firstGid = 1;
    int columns = 1;                    // số cột của ảnh tileset
    int textureId = -1;

    std::vector<TileLayer> layers;      // theo thứ tự vẽ (dưới lên trên)
    std::vector<MapObject> objects;

public:
    // Đọc file .tmj (JSON) xuất từ Tiled. textureId: ID dùng để nạp ảnh tileset vào CTextures.
    bool Load(const std::string& path, int textureId);

    // Vẽ mọi tile layer đang hiện, chỉ những ô nằm trong camera.
    void Render();

    int GetWidth()    const { return width; }
    int GetHeight()   const { return height; }
    int GetTileW()    const { return tileW; }
    int GetTileH()    const { return tileH; }
    int GetWidthPx()  const { return width * tileW; }
    int GetHeightPx() const { return height * tileH; }

    // nullptr nếu không có layer tên đó
    const TileLayer* FindLayer(const std::string& name) const;

    // Số ô tại (col, row) của layer, 0 nếu trống hoặc ra ngoài map
    int GetTile(const TileLayer* layer, int col, int row) const;

    const std::vector<MapObject>& GetObjects() const { return objects; }
};
