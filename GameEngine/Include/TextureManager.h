#pragma once
#include "TextureLoader.h"
#include <map>
namespace McEngine
{
namespace Textures
{

class TextureManager
{
public:
    static TextureManager& getInstance();

    void createTexture(std::string p_texturePath, GLenum p_wrappingType, GLenum p_drawingType, std::string p_textureLabel);

    void start();

    void shutdown();

    void setTextureIdInShader(const std::string & p_shaderLabel);

    GLuint getTexture(const std::string & p_textureLabel) const;

    void activeTexture(GLenum p_textureId, const std::string & p_textureLabel);

    void activeTextureDraft(Meshes::Mesh & p_mesh, Shaders::Shader & p_shader);

    void deactiveTexture();
private:
    TextureManager() = default;
    ~TextureManager() = default;

    std::map<std::string, GLuint> m_textures;
};
}//Textures
}//McEngine

