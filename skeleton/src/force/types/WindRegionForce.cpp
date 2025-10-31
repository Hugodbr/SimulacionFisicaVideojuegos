#include "WindRegionForce.h"

WindRegionForce::WindRegionForce(const physx::PxVec3 &direction, float strength, const physx::PxBounds3 &region)
    : WindForce(direction * strength)
    , _direction(direction.getNormalized())
    , _strength(strength)
    , _region(region)
{
}