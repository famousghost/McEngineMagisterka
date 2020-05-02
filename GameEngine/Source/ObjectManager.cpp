#include "ObjectManager.h"
#include "TextureManager.h"
#include "SphereBuilder.h"
#include "NanoSuitBuilder.h"
#include "CubeBuilder.h"
#include "ConeBuilder.h"
#include "TorusBuilder.h"
#include "CylinderBuilder.h"
#include "MonkeyHeadBuilder.h"
#include "PlaneBuilder.h"
#include <algorithm>

namespace McEngine
{
namespace Meshes
{

void ObjectManager::addObject(const Object& p_object, std::string p_objName)
{
    if (std::find_if(m_objects.begin(), m_objects.end(), 
        [&](auto& object){return object.second == p_objName;}) != m_objects.end())
    {
        LOG("Cannot add object with the same name", LogType::WARN);
        return;
    }

    m_objects.push_back(std::make_pair(p_object, p_objName));
}

void ObjectManager::addCustomObject(std::string p_objectLabel,
                                    std::string p_objectName,
                                    std::string p_shaderLabel)
{
    std::unique_ptr<ObjectBuilder> l_objectBuilder;
    if(p_objectName == "nanosuit")
    {
        l_objectBuilder = std::make_unique<NanoSuitBuilder>(p_objectName);
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "cube")
    {
        l_objectBuilder = std::make_unique<CubeBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "sphere")
    {
        l_objectBuilder = std::make_unique<SphereBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "cone")
    {
        l_objectBuilder = std::make_unique<ConeBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "torus")
    {
        l_objectBuilder = std::make_unique<TorusBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "monkeyhead")
    {
        l_objectBuilder = std::make_unique<MonkeyHeadBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "cylinder")
    {
        l_objectBuilder = std::make_unique<CylinderBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }
    else if (p_objectName == "plane")
    {
        l_objectBuilder = std::make_unique<PlaneBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addMesh();
    }

    addObject(l_objectBuilder->getObject(), p_objectLabel);
}

void ObjectManager::update(Object& p_object)
{
    setMaterialForObjectObject(p_object);

    setModelMatrixForObject(p_object);

    activeTextures(p_object);
}

void ObjectManager::setModelMatrixForObject(Object& p_object)
{
    glm::mat4 l_model;
    l_model = glm::translate(l_model, p_object.m_position);
    l_model = glm::rotate(l_model, glm::radians(p_object.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(p_object.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(p_object.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
    l_model = glm::scale(l_model, p_object.m_scale);
    p_object.m_shaderProgram->uniformMatrix4(l_model, "model");
}

void ObjectManager::setMaterialForObjectObject(Object& p_object)
{
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_ambientLight, "material.ambient");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_diffuseLight, "material.diffuse");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_specularLight, "material.specular");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_objectColor, "material.objectColor");
}

void ObjectManager::setTexture(Mesh& p_mesh,
                               const Texture& p_texture)
{
    p_mesh.m_textures.push_back(p_texture);
}

void ObjectManager::activeTextures(Object& p_object)
{
    auto& textureManager = Textures::TextureManager::getInstance();
    textureManager.setTextureIdInShader("textureShader");
    textureManager.activeTexture(GL_TEXTURE0, p_object.m_currentAvaiableTexture);
}

void ObjectManager::deleteObject(std::string p_objName)
{
    auto objectToDeleteIt = std::find_if(m_objects.begin(), m_objects.end(),
    [&](auto& object) 
    {
        return object.second == p_objName; 
    });

    if (objectToDeleteIt == m_objects.end())
    {
        LOG("Object with this name does not exist", LogType::WARN);
        return;
    }
    m_objects.erase(objectToDeleteIt);
}

std::vector<std::pair<Object, std::string>>& ObjectManager::getObjects()
{
    return m_objects;
}

}//Meshes
}//GameEngine