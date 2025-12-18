#pragma once

#include "RigidBody.h"

class FloorBody : public RigidBody
{
public:
    FloorBody(const physx::PxVec3& center, const std::string& filePath, physx::PxQuat rotation, float scale = 1.0f);
    virtual ~FloorBody() = default;

    virtual void createRenderableEntity(const std::string& filePath, float scale) override;

    virtual void onBeginUpdate(double deltaTime) override {};
    virtual void onEndUpdate(double deltaTime) override {};

protected:
    virtual void initiallize(const physx::PxVec3 &center);
};