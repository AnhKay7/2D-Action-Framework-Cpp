// json.hpp phải được include TRƯỚC Windows.h, vì Windows.h định nghĩa macro min/max làm hỏng json.hpp
#include "ThirdParty/nlohmann/json.hpp"

#include "TileMap.h"
#include <fstream>
#include <cmath>
#include <Windows.h>
#include "Engine/Game.h"
#include "Engine/Textures.h"
#include "Engine/Debug.h"

using json = nlohmann::json;

// Tiled cất cờ lật ngang/dọc/chéo ở 3-4 bit cao nhất của số ô. Bỏ đi để lấy số ô thật.
#define TILED_GID_MASK 0x0FFFFFFFu

// ---------- MapObject ----------

std::string MapObject::GetString(const std::string& key, const std::string& def) const
{
    auto it = props.find(key);
    return it == props.end() ? def : it->second;
}

int MapObject::GetInt(const std::string& key, int def) const
{
    auto it = props.find(key);
    return it == props.end() ? def : std::atoi(it->second.c_str());
}

float MapObject::GetFloat(const std::string& key, float def) const
{
    auto it = props.find(key);
    return it == props.end() ? def : (float)std::atof(it->second.c_str());
}

bool MapObject::GetBool(const std::string& key, bool def) const
{
    auto it = props.find(key);
    return it == props.end() ? def : (it->second == "true" || it->second == "1");
}

// ---------- Hàm phụ ----------

static std::wstring ToWide(const std::string& s)
{
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
    if (n <= 0) return L"";
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    w.resize(n - 1);                    // bỏ ký tự kết thúc chuỗi
    return w;
}

// "assets/tiles/map.tmj" -> "assets/tiles/"
static std::string FolderOf(const std::string& path)
{
    size_t slash = path.find_last_of("/\\");
    return slash == std::string::npos ? "" : path.substr(0, slash + 1);
}

static std::string ValueToString(const json& v)
{
    if (v.is_string())  return v.get<std::string>();
    if (v.is_boolean()) return v.get<bool>() ? "true" : "false";
    if (v.is_number_integer()) return std::to_string(v.get<long long>());
    if (v.is_number())  return std::to_string(v.get<double>());
    return v.dump();
}

// Đọc một mảng layer (gọi đệ quy cho group)
static void ReadLayers(const json& arr, int mapW, int mapH,
                       std::vector<TileLayer>& layers, std::vector<MapObject>& objects)
{
    for (const json& L : arr)
    {
        std::string type = L.value("type", "");
        bool visible = L.value("visible", true);

        if (type == "group")
        {
            if (L.contains("layers"))
                ReadLayers(L["layers"], mapW, mapH, layers, objects);
        }
        else if (type == "tilelayer")
        {
            TileLayer layer;
            layer.name = L.value("name", "");
            layer.visible = visible;

            if (L.contains("chunks"))
            {
                DebugOut(L"[MAP] Layer '%S' dang o che do Infinite. Vao Map > Map Properties bo tick Infinite.\n", layer.name.c_str());
                continue;
            }
            if (!L.contains("data") || !L["data"].is_array())
            {
                DebugOut(L"[MAP] Layer '%S' khong co data dang mang. Dat Tile layer format = CSV.\n", layer.name.c_str());
                continue;
            }

            layer.data.reserve(mapW * mapH);
            for (const json& g : L["data"])
                layer.data.push_back((int)(g.get<unsigned int>() & TILED_GID_MASK));

            layers.push_back(layer);
        }
        else if (type == "objectgroup")
        {
            for (const json& o : L["objects"])
            {
                MapObject obj;
                obj.name   = o.value("name", "");
                obj.type   = o.value("type", "");            // Tiled 1.10+: Class lưu ở "type"
                if (obj.type.empty())
                    obj.type = o.value("class", "");         // Tiled 1.9 lưu ở "class"
                obj.x      = o.value("x", 0.0f);
                obj.y      = o.value("y", 0.0f);
                obj.width  = o.value("width", 0.0f);
                obj.height = o.value("height", 0.0f);

                // Object kiểu "tile" (Insert Tile) có y là góc DƯỚI-trái, đổi về góc trên-trái
                if (o.contains("gid"))
                    obj.y -= obj.height;

                if (o.contains("properties"))
                    for (const json& p : o["properties"])
                        obj.props[p.value("name", "")] = ValueToString(p["value"]);

                objects.push_back(obj);
            }
        }
    }
}

// ---------- CTileMap ----------

bool CTileMap::Load(const std::string& path, int texId)
{
    layers.clear();
    objects.clear();
    textureId = texId;

    std::ifstream file(path);
    if (!file.is_open())
    {
        DebugOut(L"[MAP] Khong mo duoc file %S\n", path.c_str());
        return false;
    }

    json j;
    try
    {
        file >> j;
    }
    catch (const std::exception& e)
    {
        DebugOut(L"[MAP] File JSON loi: %S\n", e.what());
        return false;
    }

    width  = j.value("width", 0);
    height = j.value("height", 0);
    tileW  = j.value("tilewidth", 16);
    tileH  = j.value("tileheight", 16);

    if (j.value("infinite", false))
        DebugOut(L"[MAP] Map dang o che do Infinite, hay tat di roi xuat lai.\n");

    // ----- Tileset (chỉ hỗ trợ 1 tileset, phải Embed) -----
    if (!j.contains("tilesets") || j["tilesets"].empty())
    {
        DebugOut(L"[MAP] Map khong co tileset\n");
        return false;
    }
    if (j["tilesets"].size() > 1)
        DebugOut(L"[MAP] Map co %d tileset, chi dung cai dau tien\n", (int)j["tilesets"].size());

    const json& ts = j["tilesets"][0];
    if (ts.contains("source"))
    {
        DebugOut(L"[MAP] Tileset dang o file rieng (.tsx). Bam 'Embed Tileset' trong Tiled roi xuat lai.\n");
        return false;
    }
    firstGid = ts.value("firstgid", 1);
    columns  = ts.value("columns", 1);
    if (ts.value("tilewidth", tileW) != tileW || ts.value("tileheight", tileH) != tileH)
        DebugOut(L"[MAP] CANH BAO: o cua tileset khac o cua map\n");

    std::string imagePath = FolderOf(path) + ts.value("image", "");   // đường dẫn ảnh tính từ thư mục chứa map
    CTextures::GetInstance()->Add(textureId, ToWide(imagePath).c_str());

    // ----- Layer + object -----
    ReadLayers(j["layers"], width, height, layers, objects);

    DebugOut(L"[MAP] %S: %dx%d o, %d tile layer, %d object\n",
        path.c_str(), width, height, (int)layers.size(), (int)objects.size());
    return true;
}

const TileLayer* CTileMap::FindLayer(const std::string& name) const
{
    for (const TileLayer& L : layers)
        if (L.name == name)
            return &L;
    return nullptr;
}

int CTileMap::GetTile(const TileLayer* layer, int col, int row) const
{
    if (layer == nullptr || col < 0 || row < 0 || col >= width || row >= height)
        return 0;
    int i = row * width + col;
    return i < (int)layer->data.size() ? layer->data[i] : 0;
}

void CTileMap::Render()
{
    CGame* game = CGame::GetInstance();
    CTexture* tex = CTextures::GetInstance()->Get(textureId);
    if (tex == nullptr)
        return;

    float camX, camY;
    game->GetCamPos(camX, camY);

    // Chỉ vẽ những ô camera thấy được (+1 ô dư mỗi phía)
    int colStart = max(0, (int)floorf(camX / tileW));
    int rowStart = max(0, (int)floorf(camY / tileH));
    int colEnd   = min(width  - 1, (int)floorf((camX + game->GetBackBufferWidth())  / tileW));
    int rowEnd   = min(height - 1, (int)floorf((camY + game->GetBackBufferHeight()) / tileH));

    for (const TileLayer& L : layers)
    {
        if (!L.visible)
            continue;

        for (int row = rowStart; row <= rowEnd; row++)
            for (int col = colStart; col <= colEnd; col++)
            {
                int gid = L.data[row * width + col];
                if (gid == 0)
                    continue;

                int id = gid - firstGid;            // số thứ tự ô trong ảnh tileset
                int sx = (id % columns) * tileW;
                int sy = (id / columns) * tileH;

                // DrawRegion nhận TÂM của hình
                game->DrawRegion(col * tileW + tileW / 2.0f, row * tileH + tileH / 2.0f,
                                 tex, sx, sy, tileW, tileH);
            }
    }
}
