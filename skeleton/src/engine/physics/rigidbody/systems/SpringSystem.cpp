#include "SpringSystem.h"

#include "GameApp.h"
#include "PhysicsEngine.h"

#include "ForceManager.h"
#include "ForceGenerator.h"

#include "BoxBody.h" // TODO implement other types. Prototype in the constructor


SpringSystem::SpringSystem(
    const physx::PxVec3 &originAnchor,
    float totalLength, 
    int numRigidBodies, 
    float bodyMass, 
    float k, 
    float maxStretch, 
    const std::string& filePath, float scale
)
    : RigidBodySystem()
{
	// _emitterOrigin = origin;
    assert(numRigidBodies >= 2 && "SpringSystem must have at least 2 rigid bodies.");
    //
    _originAnchor = originAnchor;
    _bodyMass = bodyMass;
    _rigidbodies.resize(numRigidBodies);
    _d6Joints.resize(numRigidBodies + 1); // NULL<->RB0, RB0<->RB1, ..., RBn-2<->RBn-1, RBn-1<->NULL
    _internalForceGenerators.resize(numRigidBodies - 1); // No force generator for the fixed end
    _segmentLength = totalLength / (numRigidBodies - 1);
    _k = k;
    _maxStretch = maxStretch;
    _maxSegmentLength = maxStretch / (numRigidBodies - 1);

    createRigidBodies(filePath, scale);
}

SpringSystem::~SpringSystem()
{
    _d6Joints.clear();
}

void SpringSystem::init()
{
    assert(!_initialized && "SpringSystem is already initialized.");
    _initialized = true;
}

void SpringSystem::createRigidBodies(const std::string& filePath, float scale)
{
    setRenderableEntity(std::make_shared<ModelSingleMeshPBR>(filePath, scale));

    std::cout << "Dimensions of renderable entity: " << _renderableEntity->getDimensions().x << ", " << _renderableEntity->getDimensions().y << ", " << _renderableEntity->getDimensions().z << std::endl;

    _rigidbodies[0] = std::make_unique<BoxBody>(
		_originAnchor,
		_renderableEntity,
        _bodyMass
	);
	_rigidbodies[0]->init();

    _d6Joints[0] = physx::PxD6JointCreate(
        *PhysicsEngine::getInstance().getPhysics(),
        nullptr, // fixed
        physx::PxTransform(_originAnchor), // where the joint is attached in the world space
        _rigidbodies[0]->getBody(),
        physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)) // where the joint is attached in the body space
    );
    _d6Joints[0]->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);

    // Limit of anchor
    physx::PxJointLinearLimit hardLimit(
        PhysicsEngine::getInstance().getTolerancesScale(),
        0.1f
    );
    _d6Joints[0]->setLinearLimit(hardLimit);

    // Rest of the rigid bodies
    hardLimit = physx::PxJointLinearLimit(
        PhysicsEngine::getInstance().getTolerancesScale(),
        _maxSegmentLength - 2.0f * _renderableEntity->getDimensions().y
    );

    //
    for (int i = 1; i < _rigidbodies.size(); ++i) 
    {
        physx::PxVec3 position = physx::PxVec3(
            _originAnchor.x,
            _originAnchor.y - (i * _segmentLength - 2.0f * _renderableEntity->getDimensions().y),
            _originAnchor.z
        );
        _rigidbodies[i] = std::make_unique<BoxBody>(
            position,
            _renderableEntity,
            _bodyMass
        );
        _rigidbodies[i]->init();

        // Create D6 joint between previous and current rigid body
        physx::PxD6Joint* joint = _d6Joints[i];

        joint = physx::PxD6JointCreate(
            *PhysicsEngine::getInstance().getPhysics(),
            _rigidbodies[i - 1]->getBody(),
            physx::PxTransform(physx::PxVec3(0.0f, -_renderableEntity->getDimensions().y, 0.0f)), // where the joint is attached in the body space
            _rigidbodies[i]->getBody(),
            physx::PxTransform(physx::PxVec3(0.0f, _renderableEntity->getDimensions().y, 0.0f)) // where the joint is attached in the body space
        );
        joint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);

        // Lock all other DOFs
        // joint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLOCKED);
        // joint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLOCKED);
        // joint->setMotion(physx::PxD6Axis::eTWIST,  physx::PxD6Motion::eLOCKED);
        // joint->setMotion(physx::PxD6Axis::eSWING1, physx::PxD6Motion::eLOCKED);
        // joint->setMotion(physx::PxD6Axis::eSWING2, physx::PxD6Motion::eLOCKED);

        joint->setLinearLimit(hardLimit);

		//Create spring forces
		_internalForceGenerators[i - 1] = std::make_unique<SpringForce>(
			static_cast<PhysicalObject*>(_rigidbodies[i - 1].get()), // main object
			static_cast<PhysicalObject*>(_rigidbodies[i].get()), // other object
			_k, // spring constant
			_segmentLength/2.0f, // resting length
			_maxSegmentLength // max length
		);
        
    }

    // Create loose end joint
    // _looseEndJoint = physx::PxD6JointCreate(
    //     *PhysicsEngine::getInstance().getPhysics(),
    //     _rigidbodies.back()->getBody(),
    //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), // where the joint is attached in the body space
    //     nullptr,
    //     physx::PxTransform(_rigidbodies.back()->getBottomCenter()) // where the joint is attached in the world space
    // );

}

void SpringSystem::createForceGenerator()
{

}

void SpringSystem::updateInternalForces(double deltaTime)
{
    for (auto& internalForce : _internalForceGenerators) 
    {
        if (internalForce->isActive()) 
        {
            internalForce->updateForce(deltaTime);
        }
    }
}

void SpringSystem::applyForces()
{
	// Applying internal forces
	for (auto& internalForce : _internalForceGenerators) 
	{
		if (internalForce->isActive()) 
		{
			for (auto& rb : _rigidbodies) {
				rb->applyForce(*internalForce);
			}
		}
	}
}

void SpringSystem::update(double deltaTime)
{
    assert(_initialized && "SpringSystem must be initialized before update.");

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

void SpringSystem::attachObject(RigidBody* rb)
{
    _looseEndJoint->setActors(_rigidbodies.back()->getBody(), rb->getBody());
}

// void SpringSystem::setRenderable(bool renderable)
// {
// 	ParticleSystem::setRenderable(renderable);

// 	std::cout << "SpringSystem setRenderable: " << renderable << std::endl;

// 	// Update visibility of particles
// 	for (auto& [generator, pool] : _generatorsAndPools)
// 	{
// 		for (int i = 0; i < pool->getActiveCount(); ++i)
// 		{
// 			setRenderableForParticle(*pool->accessParticlePool()[i]);
// 		}
// 	}
// }
