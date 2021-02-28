#pragma once
#include "TextureLoader.h"
#include <map>

namespace McEngine
{
namespace Textures
{

namespace
{
    constexpr std::size_t CUBE_MAP_TEXTURE_SIZE = 6;
}

class TextureManager
{
public:
    static TextureManager& getInstance();

    bool createTexture(std::string p_texturePath, GLenum p_wrappingType, GLenum p_drawingType, std::string p_textureLabel);

    bool createNormalTexture(std::string p_texturePath, GLenum p_wrappingType, GLenum p_drawingType, std::string p_textureLabel);

    void start();

    std::vector<std::string> prepareDefaultCubemapTextures();

    void shutdown();

    void setTextureIdInShader(const std::string & p_shaderLabel);

    void setNormalTextureIdInShader(const std::string & p_shaderLabel);

    GLuint getTexture(const std::string & p_textureLabel) const;

    GLuint getNormalTexture(const std::string & p_textureLabel) const;

    void activeTexture(GLenum p_textureId, const std::string & p_textureLabel);

    void activeTextureById(GLenum p_textureLabel, GLuint p_textureId);

    void activeNormalTexture(GLenum p_textureId, const std::string & p_textureLabel);

    void activeCubemapTexture(Meshes::Object & p_object);

    void createCubeMapTexture(std::vector<std::string> p_texturesPath, GLenum p_wrappingType, GLenum p_drawingType);

    void activeTexturesForCustomObject(Meshes::Mesh & p_mesh, Shaders::Shader & p_shader);

    void deactiveTexture();
private:
    TextureManager() = default;
    ~TextureManager() = default;

    std::map<std::string, GLuint> m_textures;
    std::map<std::string, GLuint> m_normalTexture;
    GLuint m_cubeMapTexture;
};
}//Textures
}//McEngine

