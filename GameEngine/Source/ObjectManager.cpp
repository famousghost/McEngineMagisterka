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
        LOG("Cannot add object with the same name", McEngine::LogType::WARN);
        return;
    }

    m_objects.push_back(std::make_pair(p_object, p_objName));
}

void ObjectManager::addDefaultObject(std::string p_objectName, DefaultObjectType p_defaultObjectType)
{
    std::unique_ptr<ObjectBuilder> l_objectBuilder;
    if (p_defaultObjectType == DefaultObjectType::CUBE)
    {
        std::vector<Meshes::ColorValues> l_colorValues =
        {
            Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },

            Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },

            Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },

            Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },

            Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
            Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },

            Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
            Meshes::ColorValues{ 0.0f, 0.0f, 0.0f }
        };
        std::vector<Meshes::TextureCoords> l_textureCoords;
        l_objectBuilder = std::make_unique<Meshes::CubeBuilder>(std::make_unique<Meshes::CubeModelBuilder>());
        l_objectBuilder->prepareAndAttachColorArray(l_colorValues)
                        .prepareAndAttachTextureArray(l_textureCoords)
                        .preapreAndAttachNormalArray();
    }

    l_objectBuilder->createModel();
    l_objectBuilder->getObject().m_shaderProgram->createProgram("Shaders/colorCube.vlsl", "Shaders/colorCube.flsl");
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
        LOG("Object with this name does not exist", McEngine::LogType::WARN);
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