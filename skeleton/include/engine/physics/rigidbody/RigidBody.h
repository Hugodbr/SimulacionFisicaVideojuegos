#pragma once

#include <PxPhysicsAPI.h>
#include "PhysicalObject.h"
#include "EntityRenderable.h"

class PhysicsEngine;

// Abstract class for rigid bodies in the simulation
class RigidBody : public PhysicalObject
{
public:
    RigidBody();
    virtual ~RigidBody() = 0;

    virtual void update(double deltaTime) override;
    virtual void onBeginUpdate(double deltaTime) {}; // Hook for derived classes
    virtual void onEndUpdate(double deltaTime) {}; // Hook for derived classes

    virtual void setRenderableEntity(std::unique_ptr<Abs_Entity> renderable);
    virtual void createRenderableEntity(const std::string& filePath) {};
    virtual void load();
	virtual void unload();
	virtual void render(const glm::mat4& modelViewMat);

protected:
    virtual void integrate(double deltaTime) override {};

protected:
    // Static members shared across all RigidBody instances
    static inline PhysicsEngine* _physicsEngine;

    static inline physx::PxPhysics*  _physics  = nullptr;
    static inline physx::PxScene*    _scene    = nullptr;
    
    // Specifics for RigidBody
    physx::PxMaterial* _material = nullptr;
    physx::PxShape*    _shape    = nullptr;

    std::unique_ptr<Abs_Entity> _renderableEntity;
    physx::PxRigidActor* _body = nullptr;
};
