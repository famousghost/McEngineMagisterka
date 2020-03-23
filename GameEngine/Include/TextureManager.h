#pragma once
#include "TextureLoader.h"
#include <map>
namespace McEngine
{
namespace Textrues
{

class TextureManager
{
public:
    static TextureManager& getInstance();

    void createTexture(std::string p_texturePath, GLenum p_wrappingType, GLenum p_drawingType);

    void start();

    void setTextureIdInShader(const std::string & p_shaderLabel);

    void activeTexture();

    void deactiveTexture();

    void shutdown();
private:
    TextureManager() = default;
    ~TextureManager() = default;

    std::map<std::string, GLuint> m_textures;
};
}//Textures
}//McEngine

