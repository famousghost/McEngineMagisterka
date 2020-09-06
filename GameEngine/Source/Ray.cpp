#include "Ray.h"
namespace McEngine
{
namespace Meshes
{

Ray::Ray() 
: m_direction(0.0f, 0.0f, -1.0f)
{}

Ray::Ray(const glm::vec3 & p_origin, const glm::vec3 & p_direction)
    :m_origin(p_origin), m_direction(p_direction)
{
}

void Ray::normalizeDirection()
{
}

}//Meshes
}//McEngine