#include "CubeAABBCollisionHandler.h"

namespace McEngine
{
namespace Physics
{

bool CubeAABBCollisionHandler::checkCollision(const Meshes::Collider& p_coliderA,
                                              const Meshes::Collider& p_coliderB)
{
    return (p_coliderA.m_xSection.min.x <= p_coliderB.m_xSection.max.x
        and p_coliderA.m_xSection.max.x >= p_coliderB.m_xSection.min.x)
        and (p_coliderA.m_ySection.min.y <= p_coliderB.m_ySection.max.y
            and p_coliderA.m_ySection.max.y >= p_coliderB.m_ySection.min.y)
        and (p_coliderA.m_zSection.min.z >= p_coliderB.m_zSection.max.z
            and p_coliderA.m_zSection.max.z <= p_coliderB.m_zSection.min.z);
}

Meshes::ColMainfold CubeAABBCollisionHandler::getColMainfold() const
{
    return m_mainfold;
}

}//Physics
}//McEngine