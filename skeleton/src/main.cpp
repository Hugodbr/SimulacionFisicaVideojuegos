#include <stdlib.h>
#include <crtdbg.h>

#include <ctype.h>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"


#include "ForceManager.h"
#include "GravitationalForce.h"
#include "WindForce.h"
#include "HurricaneForce.h"

#include "CoordAxis.h"
#include "Particle.h"
#include "Bullet.h"
#include "CannonBall.h"

#include "RainSystem.h"
#include "GridSystem.h"
#include "GunSystem.h"
#include "BoxSystem.h"
#include "MeshSystem.h"


std::string display_text = "Nomemorilics";


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
std::vector<Particle*> particles;

Camera* cam = nullptr;
std::vector<ParticleSystem*> particleSystems;

ForceManager& forceManager = ForceManager::getInstance();
using sysIt = std::vector<ParticleSystem*>::iterator;
sysIt RainSystemIt;

GridSystem* gridSystem = nullptr;
GunSystem* gunSystem = nullptr;

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


	cam = GetCamera();

	// =========================================================================================
	// Global Forces
	// =========================================================================================
	// Create and register a gravitational force for all particle systems
	std::unique_ptr<GlobalForce> gravForce = std::make_unique<GravitationalForce>();
	forceManager.registerGlobalForce(std::move(gravForce));

	// =========================================================================================
	// Gun System
	// =========================================================================================
	// physx::PxVec3 gunOrigin = physx::PxVec3(8.0f, 3.0f, 0.0f);
	// gunSystem = new GunSystem(gunOrigin, cam);
	// gunSystem->init();
	// particleSystems.push_back(gunSystem);

	// =========================================================================================
	// Rain System
	// =========================================================================================
	float halfRegionSize = 100.0f;
	Region rainRegion(physx::PxBounds3(physx::PxVec3(-halfRegionSize, -halfRegionSize, -halfRegionSize), physx::PxVec3(halfRegionSize, halfRegionSize, halfRegionSize)));
	physx::PxVec3 rainOrigin = physx::PxVec3(0.0f, rainRegion.shape.box.minimum.y, 0.0f);
	RainSystem* rs = new RainSystem(rainOrigin, rainRegion);
	rs->init();
	particleSystems.push_back(rs);
	RainSystemIt = std::find(particleSystems.begin(), particleSystems.end(), rs);

	// =========================================================================================
	// Grid System
	// =========================================================================================
	// gridSystem = new GridSystem(
	// 	Region(physx::PxBounds3(physx::PxVec3(-100, -100, -100), physx::PxVec3(100, 100, 100))), 
	// 	1.0f, 
	// 	20.0,
	// 	Constants::Color::White
	// );
	// gridSystem->init();
	// gridSystem->toggleVisibility(); // Start invisible
	// particleSystems.push_back(gridSystem);

	// =========================================================================================
	// Box System
	// =========================================================================================
	// BoxSystem* boxSystem = new BoxSystem(
	// 	Region(MeshData()), 
	// 	1.0f, 
	// 	1.0,
	// 	Constants::Color::White
	// );
	// boxSystem->init();
	// particleSystems.push_back(boxSystem);

	// =========================================================================================
	// Suzanne Mesh System
	// =========================================================================================
	// MeshSystem* meshSystem = new MeshSystem(
	// 	"../resources/suzanneCenter.obj", 
	// 	1.0f, // point size
	// 	2.0, // scale
	// 	Constants::Color::White
	// );
	// meshSystem->init();
	// particleSystems.push_back(meshSystem);

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

	forceManager.update(dt);

	for (auto& ps : particleSystems) {
		ps->update(dt);
	}
	
	// std::cout << "Step Physics -> dt: " << dt << " seconds." << std::endl;
	gScene->simulate(dt);
	gScene->fetchResults(true);
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

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
		gunSystem->shoot();
		break;
	case 'G':
		gridSystem->setRenderable(!gridSystem->isRenderable());
		break;
	case 'R': 
		// Toggle rain system render state
		if (RainSystemIt != particleSystems.end()) {
			(*RainSystemIt)->setRenderable(!(*RainSystemIt)->isRenderable());
		}
		case 'E':
		// Toggle rain system active state
		if (RainSystemIt != particleSystems.end()) {
			(*RainSystemIt)->setActive(!(*RainSystemIt)->isActive());
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
	// _CrtSetBreakAlloc(524); // Uncomment to break at specific memory allocation number

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

