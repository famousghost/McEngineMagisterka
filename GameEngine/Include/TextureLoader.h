#pragma once
#include <string>
#include <ScenesManager.h>

namespace McEngine
{
namespace Textures
{
class TextureLoader
{
public:
    TextureLoader();
    ~TextureLoader();

    GLuint loadTexture(std::string p_texturePath,
                       GLenum p_wrappingType,
                       GLenum p_drawingType);

private:
    void setTextureParameters(GLenum p_wrappingType, GLenum p_drawingType);
};

}//Textrues
}//McEngine