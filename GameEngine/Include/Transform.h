#pragma once
#include "Mesh.h"

struct Transform
{
    Transform()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_orientation = glm::mat3();
        m_radiusScale = 1.0f;
    }

    bool operator==(const Transform& p_transform)
    {
        return m_position == p_transform.m_position
            and m_rotatione == p_transform.m_rotatione
            and m_scale == p_transform.m_scale;
    }

    bool operator!=(const Transform& p_transform)
    {
        return not this->operator==(p_transform);
    }

    glm::vec3 m_position;
    glm::vec3 m_rotatione;
    glm::vec3 m_scale;
    glm::mat3 m_orientation;
    float m_radiusScale;
};