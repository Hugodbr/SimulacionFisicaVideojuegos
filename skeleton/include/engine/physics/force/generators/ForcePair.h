#pragma once

#include "ForceGenerator.h"

class PhysicalObject;

class ForcePair : public virtual ForceGenerator 
{
public:
    ForcePair(PhysicalObject* mainObject, PhysicalObject* otherObject = nullptr);
    virtual ~ForcePair();

    virtual void setOtherObject(PhysicalObject* otherObject);
    // Vulnerable by default
    virtual void setObjectVulnerable(PhysicalObject* object, bool vulnerable);

protected:
    std::pair<PhysicalObject*, bool> _mainObject;
    std::pair<PhysicalObject*, bool> _otherObject;
};
