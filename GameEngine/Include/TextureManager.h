#pragma once
namespace McEngine
{
namespace Textrues
{

class TextureManager
{
public:
    static TextureManager& getInstance();
private:
    TextureManager() = default;
    ~TextureManager() = default;
};
}//Textures
}//McEngine

