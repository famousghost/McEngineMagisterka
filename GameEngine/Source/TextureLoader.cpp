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
    glBindTexture(GL_TEXTURE_2D, l_textureId);

    setTextureParameters(p_wrappingType, p_drawingType);

    unsigned char *l_image = stbi_load(p_texturePath.c_str(), &l_width, &l_height, &l_nrChannels, 0);
    if (l_image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, l_width, l_height, 0, GL_RGB, GL_UNSIGNED_BYTE, l_image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
       LOG("Cannot load texture", LogType::WARN);
    }
    stbi_image_free(l_image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return l_textureId;
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