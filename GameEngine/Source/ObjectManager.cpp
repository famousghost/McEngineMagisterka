#include "ObjectManager.h"
#include "CubeModelBuilder.h"
#include "CubeBuilder.h"
#include "TextureManager.h"
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

void ObjectManager::addDefaultObject(std::string p_objectName, 
                                     DefaultObjectType p_defaultObjectType,
                                     std::string p_shaderLabel)
{
    std::unique_ptr<ObjectBuilder> l_objectBuilder;
    if (p_defaultObjectType == DefaultObjectType::CUBE)
    {
        l_objectBuilder = std::make_unique<Meshes::CubeBuilder>();
        l_objectBuilder->addShaderProgram(p_shaderLabel).addVertexArrayObject();
    }

    addObject(l_objectBuilder->getObject(), p_objectName);
}

void ObjectManager::processObject(Object& object)
{
    auto& textureManager = Textrues::TextureManager::getInstance();

    const int WIDTH = 1270;
    const int HEIGHT = 720;

    glm::mat4 l_view;
    l_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 l_projection;
    l_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

    glm::mat4 l_model;
    l_model = glm::translate(l_model, object.m_position);
    l_model = glm::rotate(l_model, glm::radians(object.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(object.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(object.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
    l_model = glm::scale(l_model, object.m_scale);

    textureManager.setTextureIdInShader("textureShader");
    textureManager.activeTexture();
    object.m_shaderProgram->uniformMatrix4(l_model, "model");
    object.m_shaderProgram->uniformMatrix4(l_view, "view");
    object.m_shaderProgram->uniformMatrix4(l_projection, "projection");
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