#pragma once
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{
struct LineSegment
{
    glm::vec3 m_startPoint;
    glm::vec3 m_endPoint;

    LineSegment() = default;

    LineSegment(const glm::vec3& p_startPoint, const glm::vec3& p_endPoint)
        :m_startPoint(p_startPoint), m_endPoint(p_endPoint)
    {}
};

struct LineSegmentHandler
{
    static float length(const LineSegment& p_lineSegment)
    {
        return glm::length(p_lineSegment.m_endPoint - p_lineSegment.m_startPoint);
    }

    static float lengthSqr(const LineSegment& p_lineSegment)
    {
        return length(p_lineSegment) * length(p_lineSegment);
    }
};

}//Meshes
}//McEngine

