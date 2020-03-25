#include "TextureManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Textures
{
TextureManager & TextureManager::getInstance()
{
    static TextureManager textureManager;
    return textureManager;
}

void TextureManager::createTexture(std::string p_texturePath,
                                   GLenum p_wrappingType,
                                   GLenum p_drawingType,
                                   std::string p_textureLabel)
{
    TextureLoader l_textureLoader;
    GLuint l_textureId = l_textureLoader.loadTexture(p_texturePath, p_wrappingType, p_drawingType);
    m_textures[p_textureLabel] = l_textureId;
}

void TextureManager::start()
{
    createTexture("Textures/wall.jpg", GL_REPEAT, GL_LINEAR, "texture");
    createTexture("Textures/awesomeface.png", GL_REPEAT, GL_LINEAR, "texture2");
    createTexture("Textures/Face_Side1.jpg", GL_REPEAT, GL_LINEAR, "texture3");
}

void TextureManager::setTextureIdInShader(const std::string& p_shaderLabel)
{
    auto& l_shader = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);

    l_shader->uniform1I(0, "texture1");
}

GLuint TextureManager::getTexture(const std::string& p_textureLabel) const
{
    return m_textures.at(p_textureLabel);
}

void TextureManager::activeTexture(GLenum p_textureId, 
                                   const std::string& p_textureLabel)
{
    glActiveTexture(p_textureId);

    try
    {
        glBindTexture(GL_TEXTURE_2D, m_textures.at(p_textureLabel));
    }
    catch (std::exception& ex)
    {
        LOG(ex.what(), LogType::WARN);
    }
}

void TextureManager::deactiveTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureManager::shutdown()
{
    for (auto& texture : m_textures)
    {
        glDeleteTextures(1, &texture.second);
    }
}

}//Textures
}//McEngine