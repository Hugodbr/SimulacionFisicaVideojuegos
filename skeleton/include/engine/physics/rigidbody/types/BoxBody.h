#pragma once

#include "RigidBody.h"

class BoxBody : public RigidBody
{
public:
    BoxBody(const physx::PxVec3& center, const std::string& filePath);
    virtual ~BoxBody() = default;

    virtual void createRenderableEntity(const std::string& filePath) override;
    
    virtual void onBeginUpdate(double deltaTime) override {};
    virtual void onEndUpdate(double deltaTime) override {};

protected:
    physx::PxBounds3 _bounds;
};