#pragma once
#include "Object.h"
#include <vector>
#include <utility>

namespace McEngine
{
namespace Meshes
{
enum class DefaultObjectType : unsigned int
{
    CUBE = 0,
    PYRAMID = 1,
    PLANE = 2,
    SPHERE = 3,
    CAPSULE = 4,
};

class ObjectManager
{
public:
    void addObject(const Object& p_object, std::string p_objName);

    void addDefaultObject(std::string p_objectName, 
                          DefaultObjectType p_defaultObjectType, 
                          std::string p_shaderLabel);

    void deleteObject(std::string p_objName);

    void processObject(Object & object);

    std::vector<std::pair<Object, std::string>>& getObjects();
private:
    std::vector<std::pair<Object, std::string>> m_objects;

};

}//Meshes
}//GameEngine