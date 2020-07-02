#pragma once
#include "Collider.h"
#include "Light.h"

namespace McEngine
{
namespace Meshes
{

struct Object
{
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    std::list<IColiderObserver*> m_collsionObservers;
    std::vector<Collider> m_colider;
    int m_colliderNumber;
    std::string m_currentAvaiableTexture;
    Material m_material;
    Transform m_transform;
    std::string m_objectName;
    glm::vec3 m_movementDirection;
    glm::vec3 m_collsionDirection;
    float m_velocity;
    bool m_isColliding;

    void attach(IColiderObserver* p_coliderObserver)
    {
        m_collsionObservers.push_back(p_coliderObserver);
    }

    void detach(IColiderObserver* p_coliderObserver)
    {
        m_collsionObservers.remove(p_coliderObserver);
    }

    void updateTransformation(const Transform& p_transform)
    {
        for (auto& collsionObserver : m_collsionObservers)
        {
            collsionObserver->update(p_transform);
        }
    }

    Object()
    {
        m_isColliding = false;
        m_colliderNumber = 1;
        m_currentAvaiableTexture = "texture";
        m_movementDirection = glm::vec3();
        m_velocity = 5.0f;
    }
};

}//Meshes
}//McEngine

