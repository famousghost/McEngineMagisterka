#include "PrefabManager.h"
#include "CubeModelBuilder.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace McEngine
{
namespace Meshes
{

PrefabManager& PrefabManager::getInstance()
{
    static PrefabManager vaoManager;
    return vaoManager;
}

void PrefabManager::start()
{
    addDefaultMesh();
}

void PrefabManager::shutdown()
{
    m_prefabMeshes.clear();
}

void PrefabManager::addDefaultMesh()
{
    std::string l_cube = "CUBE";
    m_prefabMeshes.insert(std::make_pair(l_cube, createDefaultMesh(l_cube)));
}

void PrefabManager::loadMeshFromFile(std::string p_pathFile)
{
    auto l_fromLastSlash = p_pathFile.find_last_of("/");
    std::string l_elemetLabel = p_pathFile.substr(l_fromLastSlash +1);
    m_prefabMeshes.insert(std::make_pair(l_elemetLabel, loadMesh(p_pathFile)));
}

std::shared_ptr<Mesh> PrefabManager::getMesh(std::string p_label) const
{
    return m_prefabMeshes.at(p_label);
}

std::shared_ptr<Mesh> PrefabManager::createDefaultMesh(std::string p_label)
{
    std::unique_ptr<ModelBuilder> l_modelBuilder;
    if(p_label == "CUBE")
    {
        createCube(l_modelBuilder);
    }

    return l_modelBuilder->getMesh();
}

std::shared_ptr<Mesh> PrefabManager::loadMesh(std::string p_filePath)
{
    Assimp::Importer l_import;
    const aiScene *l_scene = l_import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!l_scene || l_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !l_scene->mRootNode)
    {
        std::string errString = l_import.GetErrorString();
        std::string errMsg = "ERROR::ASSIMP::" + errString;
        LOG(errMsg, LogType::ERR);
    }

    LOG("Correct mesh addition", LogType::INF);

    m_directory = p_filePath.substr(0, p_filePath.find_last_of('/'));

   // processNode(l_scene->mRootNode, l_scene);
   return std::make_shared<Mesh>();
}

void PrefabManager::createCube(std::unique_ptr<ModelBuilder>& p_modelBuilder)
{
    p_modelBuilder = std::make_unique<CubeModelBuilder>();

    p_modelBuilder->buildVertexCoordinatesArray();
    auto l_vertexCoordSize = p_modelBuilder->getMesh()->m_vertexCoords.size();

    p_modelBuilder->buildColorValuesArray();
    auto l_colorValueSize = p_modelBuilder->getMesh()->m_colorValues.size();

    p_modelBuilder->buildTextureCoordinatesArray();
    auto l_textureCoordsSize = p_modelBuilder->getMesh()->m_textureCoords.size();

    p_modelBuilder->buildNormalValuesArray();
    auto l_normalCoordsSize = p_modelBuilder->getMesh()->m_normalCoords.size();

    p_modelBuilder->buildIndicies();

    auto& l_vertexArray = p_modelBuilder->getMesh()->m_vertexArray;

    l_vertexArray.createVao();
    l_vertexArray.createVbo();
    l_vertexArray.createEbo();

    l_vertexArray.bindVao();
    if (l_vertexCoordSize)
    {
        l_vertexArray.addValuesToAttribPointer(0, p_modelBuilder->getMesh()->m_vertexCoords);
    }

    if (l_colorValueSize)
    {
        l_vertexArray.addValuesToAttribPointer(1, p_modelBuilder->getMesh()->m_colorValues);
    }

    if (l_textureCoordsSize)
    {
        l_vertexArray.addValuesToAttribPointer(2, p_modelBuilder->getMesh()->m_textureCoords);
    }

    if (l_normalCoordsSize)
    {
        l_vertexArray.addValuesToAttribPointer(3, p_modelBuilder->getMesh()->m_normalCoords);
    }

    if (p_modelBuilder->getMesh()->m_indicies.size())
    {
        l_vertexArray.addIndicies(p_modelBuilder->getMesh()->m_indicies);
    }
    l_vertexArray.unbindVao();
}

}//Meshes
}//McEngine
