#pragma once

#include "ForceGenerator.h"

class PhysicalObject;

class ForcePair : public virtual ForceGenerator 
{
public:
    ForcePair(PhysicalObject* mainObject, PhysicalObject* otherObject = nullptr);
    virtual ~ForcePair();

    virtual void setOtherObject(PhysicalObject* otherObject);

protected:
    PhysicalObject* _mainObject;
    PhysicalObject* _otherObject;
};
