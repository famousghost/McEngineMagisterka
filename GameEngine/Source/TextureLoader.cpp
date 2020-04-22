#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace McEngine
{
namespace Textures
{
TextureLoader::TextureLoader()
{
}


TextureLoader::~TextureLoader()
{
}

GLuint TextureLoader::loadTexture(std::string p_texturePath,
                                  GLenum p_wrappingType,
                                  GLenum p_drawingType)
{
    GLuint l_textureId;
    int l_width, l_height, l_nrChannels;

    glGenTextures(1, &l_textureId);

    unsigned char *l_image = stbi_load(p_texturePath.c_str(), &l_width, &l_height, &l_nrChannels, 0);

    if (l_image)
    {
        GLenum l_format;
        if (l_nrChannels == 1)
            l_format = GL_RED;
        else if (l_nrChannels == 3)
            l_format = GL_RGB;
        else if (l_nrChannels == 4)
            l_format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, l_textureId);
        setTextureParameters(p_wrappingType, p_drawingType);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, l_width, l_height, 0, l_format, GL_UNSIGNED_BYTE, l_image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::string wrnMsg = "Cannot load texture from path " + p_texturePath;
        LOG(wrnMsg, LogType::WARN);
    }
    stbi_image_free(l_image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return l_textureId;
}

std::vector<Meshes::Texture> TextureLoader::loadMaterialTexture(aiMaterial *p_material, 
                                                                aiTextureType p_type, 
                                                                std::string p_typeName,
                                                                std::string p_directory,
                                                                std::string p_objectName)
{
    std::vector<Meshes::Texture> l_textures;
    for (unsigned int i = 0; i < p_material->GetTextureCount(p_type); i++)
    {
        aiString l_str;
        p_material->GetTexture(p_type, i, &l_str);

        bool l_skip = false;
        for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
        {
            if (std::strcmp(m_loadedTextures[j].path.data(), l_str.C_Str()) == 0)
            {
                l_textures.push_back(m_loadedTextures[j]);
                l_skip = true;
                break;
            }
        }
        if (!l_skip)
        {
            std::string l_texturePath = l_str.C_Str();
            auto l_fromLastSlash = l_texturePath.find_last_of("\\");
            l_texturePath = "Textures/" + p_objectName + "/" + l_texturePath.substr(l_fromLastSlash + 1);
            Meshes::Texture l_texture;
            l_texture.id = loadTexture(l_texturePath, GL_REPEAT, GL_LINEAR);
            l_texture.label = p_typeName;
            l_texture.path = l_texturePath;
            l_textures.push_back(l_texture);
            m_loadedTextures.push_back(l_texture);
        }
    }
    return l_textures;
}

void TextureLoader::setTextureParameters(GLenum p_wrappingType, GLenum p_drawingType)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_wrappingType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_wrappingType);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_drawingType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_drawingType);
}

}//Textrues
}//McEngine