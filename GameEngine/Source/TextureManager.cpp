#include "TextureManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Textrues
{
TextureManager & TextureManager::getInstance()
{
    static TextureManager textureManager;
    return textureManager;
}

void TextureManager::createTexture(std::string p_texturePath,
                                   GLenum p_wrappingType,
                                   GLenum p_drawingType)
{
    TextureLoader l_textureLoader;
    std::string l_textureLabel = "texture";
    GLuint l_textureId = l_textureLoader.loadTexture(p_texturePath, p_wrappingType, p_drawingType);
    m_textures[l_textureLabel] = l_textureId;
}

void TextureManager::start()
{
    createTexture("Textures/wall.jpg", GL_REPEAT, GL_LINEAR);
}

void TextureManager::setTextureIdInShader(const std::string& p_shaderLabel)
{
    auto& l_shader = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);

    l_shader->uniform1I(0, "texture1");
}

void TextureManager::activeTexture()
{
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, m_textures.at("texture"));
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