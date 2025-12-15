#include "ForcePair.h"

#include <assert.h>

ForcePair::ForcePair(PhysicalObject* mainObject, PhysicalObject* otherObject)
    : ForceGenerator()
    , _mainObject(mainObject)
    , _otherObject(otherObject)
{
}

ForcePair::~ForcePair()
{
}

void ForcePair::setOtherObject(PhysicalObject* otherObject)
{
    _otherObject = otherObject;
}