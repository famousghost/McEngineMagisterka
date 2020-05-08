#include "PrefabManager.h"
#include "TextureLoader.h"
#include "FilePathParser.h"

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
    createScreenMesh();
    loadMeshFromFile("Objects/Cube.obj");
    loadMeshFromFile("Objects/Plane.obj");
    loadMeshFromFile("Objects/Cone.fbx");
    loadMeshFromFile("Objects/Cylinder.fbx");
    loadMeshFromFile("Objects/Monkeyhead.fbx");
    loadMeshFromFile("Objects/Sphere.fbx");
    loadMeshFromFile("Objects/Torus.fbx");
    loadMeshFromFile("Objects/Nanosuit.obj");
}

void PrefabManager::createScreenMesh()
{
    m_screenQuadMesh = std::make_shared<Mesh>();

    std::vector<glm::vec3> l_verticies = std::vector<glm::vec3>
    {
        glm::vec3(-0.99f,  0.99f, 0.0f),
        glm::vec3(-0.99f, 0.01f, 0.0f),
        glm::vec3(0.0f, 0.01f, 0.0f),
        glm::vec3(-0.99f,  0.99f, 0.0f),
        glm::vec3(0.0f, 0.01f, 0.0f),
        glm::vec3(0.0f,  0.99f, 0.0f)
    };

    std::vector<glm::vec2> l_textureCoords = std::vector<glm::vec2>
    {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    for (std::size_t i = 0; i < l_verticies.size(); ++i)
    {
        Vertex l_vertex;
        l_vertex.m_position = l_verticies.at(i);
        l_vertex.m_normalCoords = glm::vec3(0.0f, 0.0f, 0.0f);
        l_vertex.m_textureCoords = l_textureCoords.at(i);
        m_screenQuadMesh->m_verticies.push_back(l_vertex);
    }

    auto& l_vertexArray = m_screenQuadMesh->m_vertexArray;

    l_vertexArray.bindVao();

    l_vertexArray.addValuesToAttribPointer(m_screenQuadMesh->m_verticies);

    l_vertexArray.unbindVao();
}

std::shared_ptr<Mesh> PrefabManager::getScreenQuadMesh() const
{
    return m_screenQuadMesh;
}

void PrefabManager::loadMeshFromFile(std::string p_filePath)
{
    m_objectName = Utility::FilePathParser::fetchObjectName(p_filePath);
    auto l_meshes = loadMesh(p_filePath);
    if (l_meshes.empty())
    {
        LOG("Cannot load object from file", LogType::ERR);
        return;
    }

    addMeshesToMap(l_meshes);
}

void PrefabManager::addMeshesToMap(std::vector<std::shared_ptr<Mesh>>& p_meshes)
{
    std::string msg = "Map object name: " + m_objectName;
    LOG(msg, LogType::DBG);
    if (p_meshes.size() == 1)
    {
        m_prefabMeshes.insert(std::make_pair(m_objectName, p_meshes[0]));
        LOG("Correct mesh addition", LogType::INF);
        LOG("Mesh Map size: " + std::to_string(m_prefabMeshes.size()), LogType::DBG);
        return;
    }
    for (std::size_t i = 0; i < p_meshes.size(); ++i)
    {
        std::string l_label = m_objectName + "_element_" + std::to_string(i);
        m_prefabMeshes.insert(std::make_pair(l_label, p_meshes[i]));
    }
    LOG("Correct mesh addition", LogType::INF);
    LOG("Mesh Map size: " + std::to_string(m_prefabMeshes.size()), LogType::DBG);
}

std::shared_ptr<Mesh> PrefabManager::getMesh(std::string p_label) const
{
    try
    {
        return m_prefabMeshes.at(p_label);
    }
    catch (std::exception& ex)
    {
        LOG("Cannot add the same object", LogType::WARN);
        return nullptr;
    }
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
        return std::vector<std::shared_ptr<Mesh>>{};
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

void PrefabManager::loadTextures(std::shared_ptr<Mesh>& p_prefabMesh, 
                                 aiMesh* p_mesh, 
                                 const aiScene* p_scene)
{
    Textures::TextureLoader l_textureLoader;

    aiMaterial* l_material = p_scene->mMaterials[p_mesh->mMaterialIndex];

    std::vector<Texture> l_diffuseMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_DIFFUSE, "texture_diffuse", m_directory, m_objectName);
    p_prefabMesh->m_textures.insert(p_prefabMesh->m_textures.end(), l_diffuseMaps.begin(), l_diffuseMaps.end());

    std::vector<Texture> l_specularMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_SPECULAR, "texture_specular", m_directory, m_objectName);
    p_prefabMesh->m_textures.insert(p_prefabMesh->m_textures.end(), l_specularMaps.begin(), l_specularMaps.end());

    std::vector<Texture> l_normalMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_HEIGHT, "texture_normal", m_directory, m_objectName);
    p_prefabMesh->m_textures.insert(p_prefabMesh->m_textures.end(), l_normalMaps.begin(), l_normalMaps.end());

    std::vector<Texture> l_heightMaps = l_textureLoader.loadMaterialTexture(l_material, aiTextureType_AMBIENT, "texture_height", m_directory, m_objectName);
    p_prefabMesh->m_textures.insert(p_prefabMesh->m_textures.end(), l_heightMaps.begin(), l_heightMaps.end());
}

void PrefabManager::setVertexArrayForMesh(std::shared_ptr<Mesh>& p_mesh)
{
    auto& l_vertexArray = p_mesh->m_vertexArray;

    l_vertexArray.bindVao();

    l_vertexArray.addValuesToAttribPointer(p_mesh->m_verticies);

    if (p_mesh->m_indicies.size())
    {
        l_vertexArray.addIndicies(p_mesh->m_indicies);
    }

    l_vertexArray.unbindVao();
}

void PrefabManager::fillMeshValues(std::shared_ptr<Mesh>& p_prefabMesh, aiMesh* p_mesh)
{
    p_prefabMesh->m_verticies = std::vector<Vertex>(p_mesh->mNumVertices);
    for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
    {

        auto& l_vertex = p_prefabMesh->m_verticies.at(i);
        l_vertex.m_position = glm::vec3(p_mesh->mVertices[i].x,
                                        p_mesh->mVertices[i].y,
                                        p_mesh->mVertices[i].z);

        l_vertex.m_normalCoords = glm::vec3(p_mesh->mNormals[i].x,
                                            p_mesh->mNormals[i].y,
                                            p_mesh->mNormals[i].z);
        if (p_mesh->mTextureCoords[0])
        {
            l_vertex.m_textureCoords = glm::vec2(p_mesh->mTextureCoords[0][i].x,
                                                 p_mesh->mTextureCoords[0][i].y);
        }
        else
        {
            l_vertex.m_textureCoords = glm::vec2(0.0f, 0.0f);
        }
    }
}

void PrefabManager::fillIndicies(std::shared_ptr<Mesh>& p_prefabMesh, aiMesh* p_mesh)
{
    for (unsigned int i = 0; i < p_mesh->mNumFaces; ++i)
    {
        aiFace l_face = p_mesh->mFaces[i];
        for (unsigned int j = 0; j < l_face.mNumIndices; ++j)
        {
            p_prefabMesh->m_indicies.push_back(l_face.mIndices[j]);
        }
    }
}

std::shared_ptr<Mesh> PrefabManager::processMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
    std::shared_ptr<Mesh> l_prefabMesh = std::make_shared<Mesh>();

    fillMeshValues(l_prefabMesh, p_mesh);
    fillIndicies(l_prefabMesh, p_mesh);
    loadTextures(l_prefabMesh, p_mesh, p_scene);
    setVertexArrayForMesh(l_prefabMesh);

    return l_prefabMesh;
}

}//Meshes
}//McEngine
