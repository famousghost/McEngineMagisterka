#pragma once
#include "Object.h"
#include "IColiderObserver.h"
#include <vector>
#include <utility>
#include "Camera.h"

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

    void addCustomObject(std::string p_objectLabel, 
                         std::string p_objectName, 
                         std::string p_shaderLabel);

    void deleteObject(std::string p_objName);

    void update(Object & object);

    void updateCollider(Object & p_object, Cameras::Camera & p_camera);

    void transformCollider(Colider & p_collider, const glm::mat4 & p_colliderModel);

    void setTexture(Mesh & p_mesh, const Texture & p_texture);

    void activeTextures(Object& p_object);

    std::vector<std::pair<Object, std::string>>& getObjects();
    glm::vec3 m_lightPosition;
private:
    std::vector<std::pair<Object, std::string>> m_objects;
    void print(const glm::vec4 & p_vec);
    //TODO rid of this variables, created only for debuging
    glm::vec4 m_x;
    glm::vec4 m_y;
    glm::vec4 m_z;

    void setMaterialForObjectObject(Object & p_object);
    void setModelMatrixForObject(Object & p_object);
    std::vector<IColiderObserver*> m_coliderObserver;

};

}//Meshes
}//GameEngine