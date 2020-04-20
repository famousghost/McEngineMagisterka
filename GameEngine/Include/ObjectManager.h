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

    void addCustomObject(std::string p_objectLabel, 
                         std::string p_objectName, 
                         std::string p_shaderLabel);

    void deleteObject(std::string p_objName);

    void processObject(Object & object);

    void setObjectPropertiesUniform(Object & p_object);

    void setTexture(Mesh & p_mesh, const Texture & p_texture);

    void activeTextures();

    std::vector<std::pair<Object, std::string>>& getObjects();
private:
    std::vector<std::pair<Object, std::string>> m_objects;

};

}//Meshes
}//GameEngine