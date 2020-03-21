#include "TextureManager.h"
namespace McEngine
{
namespace Textrues
{

TextureManager & TextureManager::getInstance()
{
    static TextureManager textureManager;
    return textureManager;
}

}//Textures
}//McEngine