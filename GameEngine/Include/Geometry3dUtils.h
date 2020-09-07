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
    static float cmp(float x, float y);
    static Ray fromPoints(const glm::vec3& p_from, const glm::vec3& p_to);
    static bool pointOnRay(const glm::vec3 & p_point, const Ray & p_ray);
    static glm::vec3 closestPoint(const Ray & p_ray, const glm::vec3 & p_point);

    static float raycast(const Meshes::Object& p_object, const Ray& p_ray);
private:

    static float raycastSphere(const Meshes::Object & p_object, const Meshes::Collider& p_collider, const Ray & p_ray);

    static float findClosestPointOnCube(glm::vec3 p_max, glm::vec3 p_min);

    static float raycastAABB(const Meshes::Object & p_object, const Meshes::Collider& p_collider, const Ray & p_ray);

    static std::vector<float> getObjectOrientation(const Meshes::Object & p_object);

    static float raycastOBB(const Meshes::Object & p_object, const Meshes::Collider& p_collider, const Ray & p_ray);
    
};

}//Utils
}//McEngine

