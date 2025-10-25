#include <stdlib.h>
#include <crtdbg.h>

#include <ctype.h>
#include <chrono>
#include <thread>
#include <iostream>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"


#include "ForceManager.h"

#include "CoordAxis.h"
#include "Particle.h"
#include "Bullet.h"
#include "CannonBall.h"

#include "RainSystem.h"
#include "GridSystem.h"


std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//RenderItem* sphere = nullptr;

Camera* cam = nullptr;
std::vector<ParticleSystem*> particleSystems;

GridSystem* gridSystem = nullptr;


void shootParticle() {

	//physx::PxTransform pose = physx::PxTransform(cam->getEye().x, cam->getEye().y, cam->getEye().z);
	//float speed = 25.0f;
	//physx::PxVec3 velocity = physx::PxVec3(cam->getDir().x, cam->getDir().y, cam->getDir().z).getNormalized() * speed;
	//physx::PxVec3 gravity = physx::PxVec3(0, 0, 0);
	//double damping = 0.98;
	//Particle* particle = new Particle(pose, velocity, gravity, damping, Constants::Integration_Method::EULER_SEMI_IMPLICIT);

	//particles.push_back(particle);
}

void shootBullet() {
	//// ????
	//// La bala no sale de donde esta la camara

	//physx::PxTransform initTransform = physx::PxTransform(cam->getDir().x, cam->getDir().y, cam->getDir().z);
	//physx::PxVec3      initDirection = physx::PxVec3(cam->getDir().x, cam->getDir().y, cam->getDir().z).getNormalized();
	////std::cout << initDirection.x << " " << initDirection.y << " " << initDirection.z;

	//particles.push_back(new CannonBall(initTransform, initDirection, Constants::Integration_Method::VERLET));
}

void createParticles(Constants::ParticleType particleType)
{
	//switch (particleType) {
	//	case Constants::DEFAULT: {

	//		break;
	//	}
	//	case Constants::BULLET: {

	//		break;
	//	}
	//	case Constants::CANNON_BALL: {

	//		break;
	//	}
	//}
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);


	//CoordAxis* axis = new CoordAxis();

	//createParticles(Constants::DEFAULT);
	cam = GetCamera();
	//shootParticle();

	// RainSystem* rs = new RainSystem(physx::PxVec3(0, 60, 0));
	// rs->init();
	// particleSystems.push_back(rs);

	gridSystem = new GridSystem(
		physx::PxBounds3(physx::PxVec3(-100, -100, -100), physx::PxVec3(100, 100, 100)), 
		0.5f, 
		20.0,
		Constants::Color::White
	);
	gridSystem->init();
	particleSystems.push_back(gridSystem);
	
	//physx::PxShape* shape = CreateShape(PxSphereGeometry(5));
	////physx::PxTransform* transform = new PxTransform(Vector3(0, 0, 0));
	//sphere = new RenderItem(shape, Vector4(255, 0, 0, 255));
	//RegisterRenderItem(sphere);

	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double dt)
{
	PX_UNUSED(interactive);

	ForceManager& fm = ForceManager::getInstance();

	for (auto& ps : particleSystems) {
		ps->update(dt);
	}

	gScene->simulate(dt);
	gScene->fetchResults(true);
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//DeregisterRenderItem(sphere);
	//delete axis;

	for (auto* sys : particleSystems) {
		delete sys;
		sys = nullptr;
	}

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case 'B': 
		std::cout << "shootBullet" << std::endl;
		std::cout << cam->getDir().x << std::endl;
		std::cout << cam->getDir().y << std::endl;
		std::cout << cam->getDir().z << std::endl;
		shootBullet();
		break;
	case 'G':
	{
		gridSystem->toggleVisibility();
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

int main(int, const char*const*)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(number); // Uncomment to break at specific memory allocation number

#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}

