#include "PrefabManager.h"
#include "TextureLoader.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

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
    loadMeshFromFile("Objects/nanosuit.obj");
}

void PrefabManager::shutdown()
{
    m_prefabMeshes.clear();
}

void PrefabManager::addDefaultMesh()
{
    loadMeshFromFile("Objects/Cube.obj");
    loadMeshFromFile("Objects/Cone.fbx");
    loadMeshFromFile("Objects/Cylinder.fbx");
    loadMeshFromFile("Objects/MonkeyHead.fbx");
    loadMeshFromFile("Objects/Sphere.fbx");
    loadMeshFromFile("Objects/Torus.fbx");
}

void PrefabManager::loadMeshFromFile(std::string p_pathFile)
{
    auto l_fromLastSlash = p_pathFile.find_last_of("/");
    m_objectName = p_pathFile.substr(l_fromLastSlash + 1);
    auto l_findElem = m_objectName.find_first_of(".");
    m_objectName = m_objectName.substr(0, l_findElem);
    auto l_meshes = loadMesh(p_pathFile);
    if (l_meshes.size() == 1)
    {
        m_prefabMeshes.insert(std::make_pair(m_objectName, l_meshes[0]));
        LOG("Correct mesh addition", LogType::INF);
        LOG("Mesh Map size: " + std::to_string(m_prefabMeshes.size()), LogType::DBG);
        return;
    }
    for(std::size_t i = 0; i < l_meshes.size(); ++i)
    {
        std::string l_label = m_objectName + "_element_" + std::to_string(i);
        m_prefabMeshes.insert(std::make_pair(l_label, l_meshes[i]));
    }
    LOG("Correct mesh addition", LogType::INF);
    LOG("Mesh Map size: " + std::to_string(m_prefabMeshes.size()), LogType::DBG);
}

std::shared_ptr<Mesh> PrefabManager::getMesh(std::string p_label) const
{
    return m_prefabMeshes.at(p_label);
}

std::vector<std::shared_ptr<Mesh>> PrefabManager::getMeshes(std::string p_label) const
{
    std::vector<std::shared_ptr<Mesh>> l_meshes;
    for (auto& l_prefabMesh : m_prefabMeshes)
    {
        if(l_prefabMesh.first.find(p_label) != std::string::npos)
        {
            l_meshes.push_back(l_prefabMesh.second);
        }
    }
    return l_meshes;
}

std::vector<std::shared_ptr<Mesh>> PrefabManager::loadMesh(std::string p_filePath)
{
    Assimp::Importer l_import;
    const aiScene *l_scene = l_import.ReadFile(p_filePath,
                                               aiProcess_Triangulate 
                                               | aiProcess_FlipUVs);

    std::vector<std::shared_ptr<Mesh>> l_meshes;

    if (!l_scene || l_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !l_scene->mRootNode)
    {
        std::string errString = l_import.GetErrorString();
        std::string errMsg = "ERROR::ASSIMP::" + errString;
        LOG(errMsg, LogType::ERR);
    }

    m_directory = p_filePath.substr(0, p_filePath.find_last_of('/'));

    processNode(l_scene->mRootNode, l_scene, l_meshes);
    return l_meshes;
}

void PrefabManager::processNode(aiNode* p_node, 
                                const aiScene* p_scene, 
                                std::vector<std::shared_ptr<Mesh>>& p_meshes)
{
    for (unsigned int i = 0; i < p_node->mNumMeshes; i++)
    {
        aiMesh *l_mesh = p_scene->mMeshes[p_node->mMeshes[i]];
        p_meshes.push_back(std::move(processMesh(l_mesh, p_scene)));
    }

    for (unsigned int i = 0; i < p_node->mNumChildren; i++)
    {
        processNode(p_node->mChildren[i], p_scene, p_meshes);
    }
}

std::shared_ptr<Mesh> PrefabManager::processMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
    std::shared_ptr<Mesh> l_mesh = std::make_shared<Mesh>();

    for(unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
    {
        l_mesh->m_vertexCoords.emplace_back(p_mesh->mVertices[i].x,
                                            p_mesh->mVertices[i].y,
                                            p_mesh->mVertices[i].z);

        l_mesh->m_normalCoords.emplace_back(p_mesh->mNormals[i].x, 
                                            p_mesh->mNormals[i].y, 
                                            p_mesh->mNormals[i].z);

        if (p_mesh->mTextureCoords[0])
        {

            l_mesh->m_textureCoords.emplace_back(p_mesh->mTextureCoords[0][i].x, 
                                                 p_mesh->mTextureCoords[0][i].y);
        }
        else
        {
            l_mesh->m_textureCoords.emplace_back(0.0f, 0.0f);
        }
    }

    for (unsigned int i = 0; i < p_mesh->mNumFaces; ++i)
    {
        aiFace l_face = p_mesh->mFaces[i];
        for (unsigned int j = 0; j < l_face.mNumIndices; ++j)
        {
            l_mesh->m_indicies.push_back(l_face.mIndices[j]);
        }
    }

    Textures::TextureLoader l_textureLoader;

    aiMaterial* l_material = p_scene->mMaterials[p_mesh->mMaterialIndex];

    std::vector<Texture> l_diffuseMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_DIFFUSE, "texture_diffuse", m_directory, m_objectName);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_diffuseMaps.begin(), l_diffuseMaps.end());

    std::vector<Texture> l_specularMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_SPECULAR, "texture_specular", m_directory, m_objectName);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_specularMaps.begin(), l_specularMaps.end());

    std::vector<Texture> l_normalMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_HEIGHT, "texture_normal", m_directory, m_objectName);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_normalMaps.begin(), l_normalMaps.end());

    std::vector<Texture> l_heightMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_AMBIENT, "texture_height", m_directory, m_objectName);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_heightMaps.begin(), l_heightMaps.end());

    auto& l_vertexArray = l_mesh->m_vertexArray;

    l_vertexArray.createVao();
    l_vertexArray.createVbo();
    l_vertexArray.createEbo();

    l_vertexArray.bindVao();
    if (l_mesh->m_vertexCoords.size())
    {
        l_vertexArray.addValuesToAttribPointer(0, l_mesh->m_vertexCoords);
    }

    if (l_mesh->m_colorValues.size())
    {
        l_vertexArray.addValuesToAttribPointer(1, l_mesh->m_colorValues);
    }

    if (l_mesh->m_textureCoords.size())
    {
        l_vertexArray.addValuesToAttribPointer(2, l_mesh->m_textureCoords);
    }

    if (l_mesh->m_normalCoords.size())
    {
        l_vertexArray.addValuesToAttribPointer(3, l_mesh->m_normalCoords);
    }

    if (l_mesh->m_indicies.size())
    {
        l_vertexArray.addIndicies(l_mesh->m_indicies);
    }
    l_vertexArray.unbindVao();

    return l_mesh;
}

}//Meshes
}//McEngine
