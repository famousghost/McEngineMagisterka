#pragma once
#include "Ray.h"
#include "Object.h"
#include <array>

namespace McEngine
{
namespace Utils
{
namespace
{
    constexpr std::size_t MAX_MIN_VALUES_SIZE = 6;
    using Ray = Meshes::Ray;
}
class Geometry3dUtils
{
public:
    static float cmp(float x, float y);
    static Ray fromPoints(const glm::vec3& p_from, const glm::vec3& p_to);
    static bool pointOnRay(const glm::vec3 & p_point, const Ray & p_ray);
    static glm::vec3 closestPoint(const Ray & p_ray, const glm::vec3 & p_point);

    static bool raycast(const Meshes::Object & p_object, 
                         const Ray & p_ray,
                         Physics::RaycastResult * p_raycastResult = nullptr);
private:

    static bool raycastSphere(const Meshes::Object & p_object, 
                               const Meshes::Collider& p_collider, 
                               const Ray & p_ray,
                               Physics::RaycastResult* p_raycastResult = nullptr);

    static bool findClosestPointOnCubeAABB(glm::vec3 p_max,
                                           glm::vec3 p_min, 
                                           const Ray& p_ray,
                                           Physics::RaycastResult* p_raycastResult);

    static bool findClosestPointOnCubeOBB(const glm::vec3& p_orientationX,
                                          const glm::vec3& p_orientationY,
                                          const glm::vec3& p_orientationZ,
                                          const std::array<float, MAX_MIN_VALUES_SIZE>& p_t,
                                          const Ray & p_ray,
                                          Physics::RaycastResult * p_raycastResult);

    static bool raycastAABB(const Meshes::Object & p_object, 
                            const Meshes::Collider& p_collider, 
                            const Ray & p_ray,
                            Physics::RaycastResult* p_raycastResult = nullptr);

    static std::vector<float> getObjectOrientation(const Meshes::Object & p_object);

    static bool raycastOBB(const Meshes::Object & p_object, 
                           const Meshes::Collider& p_collider, 
                           const Ray & p_ray,
                           Physics::RaycastResult* p_raycastResult = nullptr);
    
};

}//Utils
}//McEngine

