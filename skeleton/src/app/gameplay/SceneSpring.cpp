#include "SceneSpring.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/physics/PhysicsEngine.h"

#include "GravitationalForce.h"
#include "ForceManager.h"
#include "ExplosionForce.h"

#include "MeshSystem.h"
#include "RainSystem.h"
#include "SpringSystem.h"

#include "EntityRenderable.h"

#include "WaterBody.h"
#include "BoxBody.h"


SceneSpring::~SceneSpring()
{
}

void SceneSpring::init()
{
	Scene::init();
/*
	// =========================================================================================
	// Spring System
	// =========================================================================================
    // std::unique_ptr<RigidBodySystem> springSystem = std::make_unique<SpringSystem>(
    //     physx::PxVec3(0.0f, 10.0f, 0.0f), // origin anchor
    //     15.0f, // total length
    //     2,   // number of rigid bodies
    //     10.0f, // body mass
    //     50.0, // spring constant k
    //     100.0f,  // TODO max stretch
    //     ROOT_DIR + "\\resources\\blender\\sphere.obj",
    //     1.0f // scale
    // );
    // springSystem->init();

    // _rigidBodySystems.push_back(std::move(springSystem));

	// PhysicsEngine::getInstance().pushRigidBodySystem(_rigidBodySystems.back().get());

    // =======================================================================
	// RigidBody Box
	// =======================================================================
    physx::PxTransform _originAnchor = physx::PxTransform(physx::PxVec3(0.0f, 1.0f, 0.0f));

	std::unique_ptr<RigidBody> boxBody1 = std::make_unique<BoxBody>(
		_originAnchor.p,
		ROOT_DIR + "\\resources\\blender\\sphere.obj", 
        0.1f
	);
	boxBody1->init();

    // =======================================================================
    physx::PxTransform _looseEnd = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));

    std::unique_ptr<RigidBody> boxBody2 = std::make_unique<BoxBody>(
		_looseEnd.p,
		ROOT_DIR + "\\resources\\blender\\sphere.obj", 
        0.1f
	);
	boxBody2->init();

    // =======================================================================
    // 1
    physx::PxD6Joint* joint = physx::PxD6JointCreate(
        *PhysicsEngine::getInstance().getPhysics(),
        NULL, // fixed
        physx::PxTransform(_originAnchor), // where the joint is attached in the world space
        boxBody1->getBody(),
        physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)) // where the joint is attached in the body space
    ); 
    joint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);

    physx::PxJointLinearLimit hardLimit(
            PhysicsEngine::getInstance().getTolerancesScale(),
            1.5f
    );

    joint->setLinearLimit(hardLimit);

    // 2
    _b1b2Joint = physx::PxD6JointCreate(
        *PhysicsEngine::getInstance().getPhysics(),
        boxBody1->getBody(),
        physx::PxTransform(physx::PxVec3(0.0f, -1.0f, 0.0f)), // where the joint is attached in the body space
        boxBody2->getBody(),
        physx::PxTransform(physx::PxVec3(0.0f, 1.0f, 0.0f)) // where the joint is attached in the body space
    ); 
    // _b1b2Joint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLIMITED);
    // _b1b2Joint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLIMITED);
    
    _b1b2Joint->setLinearLimit(hardLimit);
    //
    _b1b2Joint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLIMITED);

    // physx::PxVec3 pA = boxBody1->getBody()->getGlobalPose().p;
    // physx::PxVec3 pB = boxBody2->getBody()->getGlobalPose().p;

    // physx::PxVec3 dir = pB - pA;
    // dir.normalize();
    // physx::PxQuat q = physx::PxShortestRotation(physx::PxVec3(1,0,0), dir);
    // _b1b2Joint->setLocalPose(physx::PxJointActorIndex::eACTOR0, physx::PxTransform(physx::PxVec3(0), q));
    // _b1b2Joint->setLocalPose(physx::PxJointActorIndex::eACTOR1, physx::PxTransform(physx::PxVec3(0), q.getConjugate()));



    //===========================================================
    // 3
    // joint = physx::PxD6JointCreate(
    //     *PhysicsEngine::getInstance().getPhysics(),
    //     boxBody1->getBody(),
    //     physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), // where the joint is attached in the body space
    //     NULL, // fixed
    //     _looseEnd // where the joint is attached in the world space
    // );
    // joint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLIMITED);

    //===========================================================
    _rigidBodies.push_back(std::move(boxBody1));
    _boxBody1 = static_cast<BoxBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());
    _rigidBodies.push_back(std::move(boxBody2));
    _boxBody2 = static_cast<BoxBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

*/
}

void SceneSpring::update()
{
    // static_cast<physx::PxRigidBody*>(_rigidBodies.back().get()->getBody())->addForce(
    //     physx::PxVec3(100000.0f, .0f, 0.0f),
    //     physx::PxForceMode::eFORCE
    // );

    // physx::PxVec3 pA = _boxBody1->getBody()->getGlobalPose().p;
    // physx::PxVec3 pB = _boxBody2->getBody()->getGlobalPose().p;

    // physx::PxVec3 dir = pB - pA;
    // dir.normalize();
    // physx::PxQuat q = physx::PxShortestRotation(physx::PxVec3(1,0,0), dir);
    // _b1b2Joint->setLocalPose(physx::PxJointActorIndex::eACTOR0, physx::PxTransform(physx::PxVec3(0), q));
    // _b1b2Joint->setLocalPose(physx::PxJointActorIndex::eACTOR1, physx::PxTransform(physx::PxVec3(0), q.getConjugate()));

    // std::cout << "Applying force to the last box body." << std::endl;
	// ! physx is updating rigid bodies and particle systems at simulation step
	// TODO maybe remove from physx and update here
}
