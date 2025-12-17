#include "RopeSystem.h"
#include "GameApp.h"
#include "PhysicsEngine.h"

#include "ForceManager.h"
#include "ForceGenerator.h"
#include "SpringForce.h"

#include "RopeNodeBody.h"

RopeSystem::RopeSystem(
    const physx::PxVec3 &beginning, 
    RigidBody *beginningBody, 
    const physx::PxVec3 &beginningAttachPoint, 
    const physx::PxVec3 &end, RigidBody *endBody, const physx::PxVec3 &endAttachPoint, 
    int numRigidBodies, float bodyMass, float k, float maxStretch, const std::string &filePath, float scale)
{
    // assert(numRigidBodies >= 2 && "RopeSystem must have at least 2 rigid bodies.");
    //
    _beginning = beginning;
    _beginningBody = beginningBody;
    _beginningAttachPoint = beginningAttachPoint;

    _end = end;
    _endBody = endBody;
    _endAttachPoint = endAttachPoint;

    _attachedRigidBodies.push_back(beginningBody);
    _attachedRigidBodies.push_back(endBody);

    _direction = (end - beginning).getNormalized();
    _totalLength = (end - beginning).magnitude();
    std::cout << "Total Length of RopeSystem: " << _totalLength << std::endl;
    _segmentLength = _totalLength / (numRigidBodies + 1);
    std::cout << "Segment Length of RopeSystem: " << _segmentLength << std::endl;
    _maxStretch = maxStretch;
    _maxSegmentLength = maxStretch / (numRigidBodies + 1);

    _rigidbodies.resize(numRigidBodies);
    _d6Joints.resize(numRigidBodies + 1); // one for the fixed beginning, one for the loose end
    _internalForceGenerators.resize(numRigidBodies + 1);

    _bodyMass = bodyMass;
    _k = k;

    setRenderableEntity(std::make_shared<ModelSingleMeshPBR>(filePath, scale));

    std::cout << "Dimensions of renderable entity in RopeSystem: " << _renderableEntity->getDimensions().x << ", " << _renderableEntity->getDimensions().y << ", " << _renderableEntity->getDimensions().z << std::endl;

    //=================================================
    // Create rigid bodies and joints

    _rigidbodies[0] = std::make_unique<RopeNodeBody>(
		_beginning,
		_renderableEntity,
        _bodyMass
	);
	_rigidbodies[0]->init();

    _internalForceGenerators[0] = std::make_unique<SpringForce>(
        static_cast<PhysicalObject*>(_attachedRigidBodies.front()), // main object
        static_cast<PhysicalObject*>(_rigidbodies[0].get()), // other object
        _k, // spring constant
        // _segmentLength/2.0f, // resting length
        _segmentLength/1.2f, // resting length
        _maxSegmentLength // max length
    );
    dynamic_cast<SpringForce*>(_internalForceGenerators[0].get())->setTensionOnly(true);
    dynamic_cast<SpringForce*>(_internalForceGenerators[0].get())->setObjectVulnerable(_attachedRigidBodies.front(), false);

    //
    // Limit of anchor
    // physx::PxJointLinearLimit hardLimit(
    //     PhysicsEngine::getInstance().getTolerancesScale(),
    //     _segmentLength
    // );

    // _d6Joints[0] = physx::PxD6JointCreate(
    //     *PhysicsEngine::getInstance().getPhysics(),
    //     _attachedRigidBodies.front()->getBody(),
    //     physx::PxTransform(_beginningAttachPoint),  // where the joint is attached in the body space
    //     _rigidbodies[0]->getBody(),
    //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)) // where the joint is attached in the body space
    // );

    // _d6Joints[0]->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLIMITED);
    // _d6Joints[0]->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);
    // _d6Joints[0]->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLIMITED);
    // _d6Joints[0]->setLinearLimit(hardLimit);
    
    //
    int i = 1;
    for ( ; i < _rigidbodies.size(); ++i) 
    {
        physx::PxVec3 position = physx::PxVec3(
            _beginning.x + i * _direction.x * _segmentLength,
            _beginning.y + i * _direction.y * _segmentLength,
            _beginning.z + i * _direction.z * _segmentLength
        );
        _rigidbodies[i] = std::make_unique<RopeNodeBody>(
            position,
            _renderableEntity,
            _bodyMass
        );
        _rigidbodies[i]->init();

		//Create spring forces
		_internalForceGenerators[i] = std::make_unique<SpringForce>(
			static_cast<PhysicalObject*>(_rigidbodies[i - 1].get()), // main object
			static_cast<PhysicalObject*>(_rigidbodies[i].get()), // other object
			_k, // spring constant
			// _segmentLength/2.0f, // resting length
			_segmentLength/1.2f, // resting length
			_maxSegmentLength // max length
		);
        dynamic_cast<SpringForce*>(_internalForceGenerators[i].get())->setTensionOnly(true);

        // _d6Joints[i] = physx::PxD6JointCreate(
        //     *PhysicsEngine::getInstance().getPhysics(),
        //     _rigidbodies[i - 1]->getBody(),
        //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),  // where the joint is attached in the body space
        //     _rigidbodies[i]->getBody(),
        //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)) // where the joint is attached in the body space
        // );

        // _d6Joints[i]->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLIMITED);
        // _d6Joints[i]->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);
        // _d6Joints[i]->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLIMITED);
        // _d6Joints[i]->setLinearLimit(hardLimit);
    }

    // _rigidbodies[i] = std::make_unique<BoxBody>(
    //     _end,
    //     _renderableEntity,
    //     _bodyMass
    // );
    // _rigidbodies[i]->init();

    // Create end joint
    // _endJoint = physx::PxD6JointCreate(
    //     *PhysicsEngine::getInstance().getPhysics(),
    //     _rigidbodies.back()->getBody(),
    //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), // where the joint is attached in the body space
    //     _endBody->getBody(),
    //     physx::PxTransform(_endAttachPoint) // where the joint is attached in the body space
    // );
    // _d6Joints[1] = _endJoint;
    // _d6Joints[1]->setLinearLimit(hardLimit);

    _internalForceGenerators[i] = std::make_unique<SpringForce>(
        static_cast<PhysicalObject*>(_rigidbodies[i - 1].get()), // main object
        static_cast<PhysicalObject*>(_attachedRigidBodies.back()), // other object
        _k, // spring constant
        // _segmentLength/2.0f, // resting length
        _segmentLength/1.2f, // resting length
        _maxSegmentLength // max length
    );
    dynamic_cast<SpringForce*>(_internalForceGenerators[i].get())->setTensionOnly(true);
    dynamic_cast<SpringForce*>(_internalForceGenerators[i].get())->setObjectVulnerable(_attachedRigidBodies.back(), false);

    // _d6Joints[i] = physx::PxD6JointCreate(
    //     *PhysicsEngine::getInstance().getPhysics(),
    //     _rigidbodies[i - 1]->getBody(),
    //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),  // where the joint is attached in the body space
    //     _attachedRigidBodies.back()->getBody(),
    //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)) // where the joint is attached in the body space
    // );

    // _d6Joints[i]->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLIMITED);
    // _d6Joints[i]->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);
    // _d6Joints[i]->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLIMITED);
    // _d6Joints[i]->setLinearLimit(hardLimit);

    // _internalForceGenerators[++i] = std::make_unique<SpringForce>(
    //     static_cast<PhysicalObject*>(_rigidbodies[i].get()), // main object
    //     static_cast<PhysicalObject*>(_attachedRigidBodies[1]), // other object
    //     _k * 1.0f, // spring constant
    //     // _segmentLength/2.0f, // resting length
    //     _segmentLength, // resting length
    //     _maxSegmentLength // max length
    // );

}

RopeSystem::~RopeSystem()
{
    _d6Joints.clear();
}

void RopeSystem::init()
{
    assert(!_initialized && "RopeSystem is already initialized.");
    _initialized = true;
}

void RopeSystem::updateInternalForces(double deltaTime)
{
    for (auto& internalForce : _internalForceGenerators) 
    {
        if (internalForce->isActive()) 
        {
            internalForce->updateForce(deltaTime);
        }
    }
}

void RopeSystem::applyForces()
{
	// Applying internal forces
	for (auto& internalForce : _internalForceGenerators) 
	{
		if (internalForce->isActive()) 
		{
			for (auto& rb : _rigidbodies) {
				rb->applyForce(*internalForce);
			}
            // for (auto& attachedRb : _attachedRigidBodies) {
            //     attachedRb->applyForce(*internalForce);
            // }
		}
	}
}

void RopeSystem::update(double deltaTime)
{
    assert(_initialized && "RopeSystem must be initialized before update.");

	// ! IMPORTANT ! Must call base update to handle sub-systems and inside forces 
    // TODO should RigidBodySystem have sub-systems?
	RigidBodySystem::update(deltaTime);
	if (!isActive()) {
		return;
	}

	// Update internal forces
	updateInternalForces(deltaTime);
	// Apply forces to all bodies
	applyForces();

	for (auto& rb : _rigidbodies) {
		rb->update(deltaTime);
	}
}
