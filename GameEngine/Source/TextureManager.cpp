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

bool TextureManager::createTexture(std::string p_texturePath,
                                   GLenum p_wrappingType,
                                   GLenum p_drawingType,
                                   std::string p_textureLabel)
{
    TextureLoader l_textureLoader;
    GLuint l_textureId = l_textureLoader.loadTexture(p_texturePath, 
                                                     p_wrappingType, 
                                                     p_drawingType);
    if (l_textureId == -1)
    {
        return false;
    }
    m_textures[p_textureLabel] = l_textureId;

    return true;
}

bool TextureManager::createNormalTexture(std::string p_texturePath,
                                         GLenum p_wrappingType,
                                         GLenum p_drawingType,
                                         std::string p_textureLabel)
{
    TextureLoader l_textureLoader;
    GLuint l_textureId = l_textureLoader.loadTexture(p_texturePath,
                                                     p_wrappingType,
                                                     p_drawingType);
    if (l_textureId == -1)
    {
        return false;
    }

    m_normalTexture[p_textureLabel] = l_textureId;

    return true;
}

void TextureManager::start()
{
    auto wallId = createTexture("Textures/wall.jpg", GL_REPEAT, GL_LINEAR, "Wall");
    auto awesomefaceId = createTexture("Textures/awesomeface.png", GL_REPEAT, GL_LINEAR, "Awsomeface");
    auto Face_Side1Id = createTexture("Textures/Face_Side1.jpg", GL_REPEAT, GL_LINEAR, "Face_Side1");
    createCubeMapTexture(prepareDefaultCubemapTextures(), GL_CLAMP_TO_EDGE, GL_LINEAR);

    if (wallId == -1)
    {
        LOG("Wall texture not exists", LogType::ERR);
        exit(0);
    }
    if (awesomefaceId == -1)
    {
        LOG("awesomefaceId texture not exists", LogType::ERR);
        exit(0);
    }
    if (Face_Side1Id == -1)
    {
        LOG("Face_Side1Id texture not exists", LogType::ERR);
        exit(0);
    }
}

std::vector<std::string> TextureManager::prepareDefaultCubemapTextures()
{
    return std::vector<std::string>
    {
        "Textures/Cubemap/xpos.jpg",
        "Textures/Cubemap/xneg.jpg",
        "Textures/Cubemap/ypos.jpg",
        "Textures/Cubemap/yneg.jpg",
        "Textures/Cubemap/zpos.jpg",
        "Textures/Cubemap/zneg.jpg"
    };
}

void TextureManager::setTextureIdInShader(const std::string& p_shaderLabel)
{
    auto l_shader = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);

    l_shader->uniform1I(0, "texture1");
}

void TextureManager::setNormalTextureIdInShader(const std::string& p_shaderLabel)
{
    auto l_shader = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);

    l_shader->uniform1I(1, "normalTexture");
}

GLuint TextureManager::getTexture(const std::string& p_textureLabel) const
{
    try
    {
        return m_textures.at(p_textureLabel);
    }
    catch (...)
    {
        std::string l_errMsg = "Cannot find texture in texture map";
        LOG(l_errMsg, LogType::ERR);
    }
}

GLuint TextureManager::getNormalTexture(const std::string& p_textureLabel) const
{
    try
    {
        return m_normalTexture.at(p_textureLabel);
    }
    catch (...)
    {
        std::string l_errMsg = "Cannot find texture in normal map";
        LOG(l_errMsg, LogType::ERR);
    }
}

void TextureManager::activeTexture(GLenum p_textureId, 
                                   const std::string& p_textureLabel)
{
    try
    {
        auto l_texture = m_textures.at(p_textureLabel);
        glActiveTexture(p_textureId);
        glBindTexture(GL_TEXTURE_2D, l_texture);
    }
    catch (...)
    {
        std::string l_errMsg = "Cannot find texture in map textureLabel = " + p_textureLabel;
        LOG(l_errMsg, LogType::WARN);
    }
}

void TextureManager::activeTextureById(GLenum p_textureLabel, GLuint p_textureId)
{
    glActiveTexture(p_textureLabel);
    glBindTexture(GL_TEXTURE_2D, p_textureId);
}

void TextureManager::activeNormalTexture(GLenum p_textureId,
                                         const std::string& p_textureLabel)
{
    try
    {
        auto l_texture = m_normalTexture.at(p_textureLabel);
        glActiveTexture(p_textureId);
        glBindTexture(GL_TEXTURE_2D, l_texture);
    }
    catch (...)
    {
        std::string l_errMsg = "Cannot find texture in map textureLabel = " + p_textureLabel;
        LOG(l_errMsg, LogType::WARN);
    }
}

void TextureManager::activeCubemapTexture(Meshes::Object& p_object)
{
    int textureSize = 0;
    for (auto& l_mesh : p_object.m_meshes)
    {
        textureSize += l_mesh->m_textures.size();
    }
    p_object.m_shaderProgram->uniform1I(textureSize+1, "cubemap");
    glActiveTexture(GL_TEXTURE0+textureSize+1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
}

void TextureManager::createCubeMapTexture(std::vector<std::string> p_texturesPath,
                                          GLenum p_wrappingType,
                                          GLenum p_drawingType)
{
    TextureLoader l_textureLoader;
    m_cubeMapTexture = l_textureLoader.loadCubeMapTexture(p_texturesPath, 
                                                          p_wrappingType, 
                                                          p_drawingType);
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
        glBindTexture(GL_TEXTURE_2D, l_textures[i].id);
        std::string l_number;
        std::string l_name = l_textures[i].label;
        if (l_name == "texture_diffuse")
            l_number = std::to_string(diffuseNr++);
        else if (l_name == "texture_specular")
            l_number = std::to_string(specularNr++);

        p_shader.uniform1I(i, ("textureMaterial." + l_name + l_number).c_str());
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