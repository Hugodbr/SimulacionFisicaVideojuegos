#pragma once

#include "RigidBodySystem.h"
#include "RigidBody.h"
#include "force/types/SpringForce.h"

class RopeSystem : public RigidBodySystem
{
public:
	RopeSystem(
        const physx::PxVec3 &beginning, RigidBody* beginningBody, const physx::PxVec3 &beginningAttachPoint,
        const physx::PxVec3 &end, RigidBody* endBody, const physx::PxVec3 &endAttachPoint,
        int numRigidBodies, float bodyMass, float k, float maxStretch, const std::string& filePath, float scale = 1.0f
    );

	~RopeSystem();

	void init() override;
	void update(double deltaTime) override;

protected:
    virtual void updateInternalForces(double deltaTime);
	// Clear all forces from the particles and apply forces from ForceManager
	virtual void applyForces() override;

protected:

    physx::PxVec3 _beginning;
    physx::PxVec3 _end;
    RigidBody* _beginningBody = nullptr;
    RigidBody* _endBody = nullptr;
    physx::PxVec3 _beginningAttachPoint;
    physx::PxVec3 _endAttachPoint;

    physx::PxVec3 _direction;

    float _totalLength;
    float _segmentLength; // rest length of each spring segment
    float _maxStretch; // maximum stretch allowed
    float _maxSegmentLength; // maximum length of each segment
    float _k; // spring constant

    float _bodyMass; // mass of each rigid body
    //
    std::vector<physx::PxD6Joint*> _d6Joints;
    physx::PxD6Joint* _endJoint = nullptr;
    //
    std::vector<std::unique_ptr<ForceGenerator>> _internalForceGenerators;
    std::vector<RigidBody*> _attachedRigidBodies;
};
