#include "ForcePair.h"

#include <assert.h>

ForcePair::ForcePair(PhysicalObject* mainObject, PhysicalObject* otherObject)
    : ForceGenerator()
    , _mainObject(mainObject, true)
    , _otherObject(otherObject, true)
{
}

ForcePair::~ForcePair()
{
}

void ForcePair::setOtherObject(PhysicalObject* otherObject)
{
    _otherObject.first = otherObject;
}

void ForcePair::setObjectVulnerable(PhysicalObject* object, bool vulnerable)
{
    if (object == _mainObject.first) {
        _mainObject.second = vulnerable;
    } else if (object == _otherObject.first) {
        _otherObject.second = vulnerable;
    } else {
        assert(false && "ForcePair::setObjectVulnerable() - Object not part of this ForcePair!");
    }   
}