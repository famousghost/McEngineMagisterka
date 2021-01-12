#pragma once
#include "Transform.h"

namespace McEngine
{
namespace Meshes
{

class IColiderObserver
{
public:
    virtual void update(const Transform & p_transformation) = 0;
};

}//Meshs
}//McEngine

