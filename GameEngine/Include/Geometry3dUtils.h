#pragma once
#include "Ray.h"
#include "Object.h"
#include "ColMainfold.h"
#include "LineSegment.h"
#include "Plane.h"
#include "Interval.h"
#include <array>

namespace McEngine
{
namespace Utils
{
namespace
{
    constexpr std::size_t MAX_MIN_VALUES_SIZE = 6;
    using Ray = Meshes::Ray;
    using ColMainfold = Meshes::ColMainfold;
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

    static bool clipToPlane(const Meshes::Plane & p_plane, 
                            const Meshes::LineSegment & p_line, 
                            glm::vec3 * p_outPoint);

    static bool checkIfPointIsOnCubeOBB(const Meshes::Object & p_object, 
                                        const glm::vec3 & p_point);

    static glm::vec3 findClosestPointOnCubeOBB(const Meshes::Object & p_object, 
                                               const glm::vec3 & p_point);

    static Meshes::Interval getIntervalABB(const Meshes::Object & p_object, 
                                           const glm::vec3 & p_axis);

    static Meshes::Interval getIntervalOBB(const Meshes::Object& p_object, 
                                           const glm::vec3& p_axis);

    static void applyImpulse(Meshes::Object& p_objectA, 
                             Meshes::Object& p_objectB,
                             Meshes::ColMainfold& p_mainfold,
                             int p_c);
    static void applyRotationeImpulse(Meshes::Object & p_object, 
                                      const glm::vec3 & p_point, 
                                      const glm::vec3 & p_impulse);
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

    static bool raycastOBB(const Meshes::Object & p_object, 
                           const Meshes::Collider& p_collider, 
                           const Ray & p_ray,
                           Physics::RaycastResult* p_raycastResult = nullptr);
    
};

}//Utils
}//McEngine

