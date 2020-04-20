#include "PrefabManager.h"
#include "TextureLoader.h"
#include "CubeModelBuilder.h"

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
    loadMeshFromFile("Objects/nanosuit.fbx");
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
    std::string l_elemetLabel = p_pathFile.substr(l_fromLastSlash + 1);
    auto l_meshes = loadMesh(p_pathFile);
    for(std::size_t i = 0; i < l_meshes.size(); ++i)
    {
        std::string l_label = l_elemetLabel + "_element_" + std::to_string(i);
        m_prefabMeshes.insert(std::make_pair(l_label, l_meshes[i]));
    }
    for (auto& l_prefabMesh : m_prefabMeshes)
    {
        LOG(l_prefabMesh.first, LogType::DBG);
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

std::shared_ptr<Mesh> PrefabManager::createDefaultMesh(std::string p_label)
{
    std::unique_ptr<ModelBuilder> l_modelBuilder;
    if(p_label == "CUBE")
    {
        createCube(l_modelBuilder);
    }

    return l_modelBuilder->getMesh();
}

std::vector<std::shared_ptr<Mesh>> PrefabManager::loadMesh(std::string p_filePath)
{
    Assimp::Importer l_import;
    const aiScene *l_scene = l_import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

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
        p_meshes.push_back(processMesh(l_mesh, p_scene));
    }

    for (unsigned int i = 0; i < p_node->mNumChildren; i++)
    {
        processNode(p_node->mChildren[i], p_scene, p_meshes);
    }
}

std::shared_ptr<Mesh> PrefabManager::processMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
    std::shared_ptr<Mesh> l_mesh = std::make_shared<Mesh>();
    std::vector<VertexCoords> l_position;
    std::vector<NormalCoords> l_normalCoords;
    std::vector<ColorValues> l_colorValues;
    std::vector<TextureCoords> l_textureCoords;
    std::vector<uint32_t> l_indicies;
    std::vector<Texture> l_textures;

    for(std::size_t i = 0; i < p_mesh->mNumVertices; ++i)
    {
        VertexCoords l_vertCoords;
        l_vertCoords.x = p_mesh->mVertices[i].x;
        l_vertCoords.y = p_mesh->mVertices[i].y;
        l_vertCoords.z = p_mesh->mVertices[i].z;

        l_mesh->m_vertexCoords.push_back(l_vertCoords);
        
        NormalCoords l_normalCoords;
        l_normalCoords.x = p_mesh->mNormals[i].x;
        l_normalCoords.y = p_mesh->mNormals[i].y;
        l_normalCoords.z = p_mesh->mNormals[i].z;

        l_mesh->m_normalCoords.push_back(l_normalCoords);

        if (p_mesh->mTextureCoords[0])
        {
            TextureCoords l_textureCoords;
            l_textureCoords.x = p_mesh->mTextureCoords[0][i].x;
            l_textureCoords.y = p_mesh->mTextureCoords[0][i].y;

            l_mesh->m_textureCoords.push_back(l_textureCoords);
        }
    }

    for (std::size_t i = 0; i < p_mesh->mNumFaces; ++i)
    {
        aiFace l_face = p_mesh->mFaces[i];
        for (std::size_t j = 0; j < l_face.mNumIndices; ++j)
        {
            l_mesh->m_indicies.push_back(l_face.mIndices[j]);
        }
    }

    Textures::TextureLoader l_textureLoader;

    aiMaterial* l_material = p_scene->mMaterials[p_mesh->mMaterialIndex];

    std::vector<Texture> l_diffuseMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_DIFFUSE, "texture_diffuse", m_directory);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_diffuseMaps.begin(), l_diffuseMaps.end());

    std::vector<Texture> l_specularMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_SPECULAR, "texture_specular", m_directory);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_specularMaps.begin(), l_specularMaps.end());

    std::vector<Texture> l_normalMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_HEIGHT, "texture_normal", m_directory);
    l_mesh->m_textures.insert(l_mesh->m_textures.end(), l_normalMaps.begin(), l_normalMaps.end());

    std::vector<Texture> l_heightMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_AMBIENT, "texture_height", m_directory);
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
