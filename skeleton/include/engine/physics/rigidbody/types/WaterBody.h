#pragma once

#include "RigidBody.h"


class WaterBody : public RigidBody
{
public:
    WaterBody(const physx::PxVec3& topCenter, const std::string& filePath, float scale = 1.0f);
    virtual ~WaterBody() = default;

    virtual void createRenderableEntity(const std::string& filePath, float scale) override;

    virtual void onBeginUpdate(double deltaTime) override {};
    virtual void onEndUpdate(double deltaTime) override;

    // Callbacks
    // virtual void onCollision(physx::PxActor* other) override; // Is trigger
    virtual void onTriggerEnter(physx::PxActor* other) override;
    virtual void onTriggerExit(physx::PxActor* other) override;

protected:
    virtual void resolveInteractions();
    // Calculate buoyant force on affected body. Linear and quadratic drag forces are also calculated.
    virtual physx::PxVec3 calculateBuoyancy(RigidBody& affectedBody);

protected:
    std::vector<physx::PxRigidActor*> _affectedRigidActors = {};

    physx::PxVec3 _topLevel;
    physx::PxReal _Cd = 1.1f;
};