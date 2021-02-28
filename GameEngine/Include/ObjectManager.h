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
                         std::string p_shaderLabel, 
                         ColliderType p_defaultColliderType);

    void addCustomObject(std::string p_objectLabel, 
                         std::string p_objectName, 
                         std::string p_shaderLabel);

    void addTerrain(std::string p_objectLabel, std::string p_shaderLabel, ColliderType p_defaultColliderType);

    void addSkyBox();

    void deleteObject(std::string p_objName);

    std::string cloneObject(std::string p_objName);

    Object & getSkybox();

    void update(Object & object);

    void moveObject(Object & p_object);

    void updateCollider(Object & p_object, Cameras::Camera & p_camera);

    void debugMatrix(const glm::mat4 & p_mat);

    void transformCollider(Collider & p_collider, const glm::mat4 & p_colliderModel);

    void setTexture(Mesh & p_mesh, const Texture & p_texture);

    void activeTextures(Object& p_object);

    std::vector<std::pair<Object, std::string>>& getObjects();
    glm::vec3 m_lightPosition;
    glm::vec2 l_nearFarPlane = glm::vec2(1.0, 7.5);
private:
    void resetValues(Object & p_object);

    void dbgVector(const glm::vec3 & p_vec, const std::string & p_msg);

    std::vector<std::pair<Object, std::string>> m_objects;
    Object m_skyBox;

    void setMaterialForObject(Object & p_object);
    void setSpecialUniformsForObject(Object & p_object);
    void setModelMatrixForObject(Object & p_object);
    void initState(Object & p_object);
    glm::vec3 getEulerAngelsFromTransform(const glm::vec3 & p_rotatione);
    glm::mat4 getRotationeMatrixFromEulerAngels(const glm::vec3 & p_eulerAngels);
    void scaleObjectSize(Object & p_object);
    std::vector<IColiderObserver*> m_coliderObserver;

};

}//Meshes
}//GameEngine