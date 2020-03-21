#include "ObjectManager.h"
#include "CubeModelBuilder.h"
#include "CubeBuilder.h"
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

void ObjectManager::addDefaultObject(std::string p_objectName, DefaultObjectType p_defaultObjectType)
{
    std::unique_ptr<ObjectBuilder> l_objectBuilder;
    if (p_defaultObjectType == DefaultObjectType::CUBE)
    {
        l_objectBuilder = std::make_unique<Meshes::CubeBuilder>();
        l_objectBuilder->addShaderProgram("defaultShader").addVertexArrayObject();
    }

    addObject(l_objectBuilder->getObject(), p_objectName);
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