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
    virtual void createRenderableEntity(const std::string& filePath, float scale = 1.0f) {};
    virtual void load();
	virtual void unload();
	virtual void render(const glm::mat4& modelViewMat);

    // Callbacks
    virtual void onCollision(physx::PxActor* other) {};
    virtual void onTriggerEnter(physx::PxActor* other) {};
    virtual void onTriggerExit(physx::PxActor* other) {};

    // TODO OVERRIDE IN RIDIG BODY TO GET FROM POSE PHYSX

    // Getters
    physx::PxRigidActor* getBody() const { return _body; }
    physx::PxTransform getPose() const { return _body->getGlobalPose(); }
    physx::PxBounds3 getBounds() const { return _body->getWorldBounds(); }
    physx::PxVec3 getTopCenter() const;
    physx::PxVec3 getBottomCenter() const;
    float getVolume() const { return _volume; }

protected:
    virtual void integrate(double deltaTime) override {};
    virtual void updateRenderableEntityPose();

    // Call only before simulation starts
    virtual void rotate(const physx::PxQuat& deltaRotation);
    // Calculate volume before simulation starts
    virtual void calculateVolume(const glm::vec3& dimensions);

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

    float _volume;
    physx::PxReal _density;
};
