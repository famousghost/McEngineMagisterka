#pragma once
#include "IColiderObserver.h"
#include "Object.h"

namespace McEngine
{
namespace Meshes
{

class ColiderObserver : public IColiderObserver
{
public:
    ColiderObserver(Object& p_object);
    void update(const Transform & p_transformation) override;

private:
    Object& m_object;
    Transform m_transform;
};

}//Meshs
}//McEngine

