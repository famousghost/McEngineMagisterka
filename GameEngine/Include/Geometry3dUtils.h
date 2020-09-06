#pragma once
#include "Ray.h"
#include "Object.h"

namespace McEngine
{
namespace Utils
{
namespace
{
    using Ray = Meshes::Ray;
}
class Geometry3dUtils
{
public:
    static Ray fromPoints(const glm::vec3& p_from, const glm::vec3& p_to);
    static bool pointOnRay(const glm::vec3 & p_point, const Ray & p_ray);
    static glm::vec3 closestPoint(const Ray & p_ray, const glm::vec3 & p_point);

    static float raycast(const Meshes::Object& p_object, const Ray& p_ray);
    
};

}//Utils
}//McEngine

