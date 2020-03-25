#include "TextureLoader.h"
#include <SOIL.h>


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
    int l_width, l_height;
    unsigned char* l_image;

    glGenTextures(1, &l_textureId);
    glBindTexture(GL_TEXTURE_2D, l_textureId);

    setTextureParameters(p_wrappingType, p_drawingType);

    l_image = SOIL_load_image(p_texturePath.c_str(), &l_width, &l_height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, l_width, l_height, 0, GL_RGB, GL_UNSIGNED_BYTE, l_image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(l_image);
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