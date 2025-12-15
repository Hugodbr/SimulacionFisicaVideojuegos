#include "SpringTestScene.h"

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


SpringTestScene::~SpringTestScene()
{
}

void SpringTestScene::init()
{
	Scene::init();

	// =========================================================================================
	// Spring System
	// =========================================================================================
    float totalLength = 2.0f;
    std::unique_ptr<RigidBodySystem> springSystem = std::make_unique<SpringSystem>(
        physx::PxVec3(0.0f, 2.0f, 0.0f), // origin anchor
        totalLength, // total length
        5,   // number of rigid bodies
        10.0f, // body mass
        10.0f, // spring constant k
        2.0f * totalLength,  // TODO max stretch
        ROOT_DIR + "\\resources\\blender\\sphere.obj",
        0.1f // scale
    );
    springSystem->init();

    _rigidBodySystems.push_back(std::move(springSystem));

	PhysicsEngine::getInstance().pushRigidBodySystem(_rigidBodySystems.back().get());


}

void SpringTestScene::update()
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
