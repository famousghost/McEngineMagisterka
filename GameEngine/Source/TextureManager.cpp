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
    createTexture("Textures/wall.jpg", GL_REPEAT, GL_LINEAR, "Wall");
    createTexture("Textures/awesomeface.png", GL_REPEAT, GL_LINEAR, "Awsomeface");
    createTexture("Textures/Face_Side1.jpg", GL_REPEAT, GL_LINEAR, "Face_Side1");
}

void TextureManager::setTextureIdInShader(const std::string& p_shaderLabel)
{
    auto l_shader = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);

    l_shader->uniform1I(0, "texture1");
}

GLuint TextureManager::getTexture(const std::string& p_textureLabel) const
{
    return m_textures.at(p_textureLabel);
}

void TextureManager::activeTexture(GLenum p_textureId, 
                                   const std::string& p_textureLabel)
{
    try
    {
        glActiveTexture(p_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textures.at(p_textureLabel));
    }
    catch (std::exception& ex)
    {
        LOG(ex.what(), LogType::WARN);
    }
}

void TextureManager::activeTexturesForCustomObject(Meshes::Mesh& p_mesh,
                                                   Shaders::Shader& p_shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    auto& l_textures = p_mesh.m_textures;
    for (std::size_t i = 0; i < l_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string l_number;
        std::string l_name = l_textures[i].label;
        if (l_name == "texture_diffuse")
            l_number = std::to_string(diffuseNr++);
        else if (l_name == "texture_specular")
            l_number = std::to_string(specularNr++);

        p_shader.uniform1I(i, ("textureMaterial." + l_name + l_number).c_str());
        glBindTexture(GL_TEXTURE_2D, l_textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
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