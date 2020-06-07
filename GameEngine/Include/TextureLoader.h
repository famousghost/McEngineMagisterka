#pragma once
#include <string>
#include <ScenesManager.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace McEngine
{
namespace Textures
{
class TextureLoader
{
public:
    TextureLoader();
    ~TextureLoader();

    GLuint loadCubeMapTexture(std::vector<std::string> p_texturesPath, GLenum p_wrappingType, GLenum p_drawingType);

    GLuint loadTexture(std::string p_texturePath,
                       GLenum p_wrappingType,
                       GLenum p_drawingType);

    std::vector<Meshes::Texture> loadMaterialTexture(aiMaterial * p_material, 
                                                     aiTextureType p_type,
                                                     std::string p_typeName, 
                                                     std::string p_directory,
                                                     std::string p_objectName);

private:
    void setTextureParameters(GLenum p_wrappingType, GLenum p_drawingType);

    void setCubeMapTextureParameters(GLenum p_wrappingType, GLenum p_drawingType);

    std::vector<Meshes::Texture> m_loadedTextures;
};

}//Textrues
}//McEngine