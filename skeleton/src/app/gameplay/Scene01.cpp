#include "Scene01.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Particle.h"
#include "engine/physics/PhysicsEngine.h"

Scene01::~Scene01()
{
	// if (_particle != nullptr) {
	// 	delete _particle;
	// 	_particle = nullptr;
	// }

	// if (_particle2 != nullptr) {
	// 	delete _particle2;
	// 	_particle2 = nullptr;
	// }
}
void Scene01::init()
{
	Scene::init();

	// _particle = new Particle(
	// 	physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), 
	// 	physx::PxVec3(0.0f, 0.0f, 0.0f), 
	// 	physx::PxVec3(0.0f, -9.8f, 0.0f), 
	// 	1.0f, 
	// 	Integration_Method::VERLET,
	// 	50.0f);

	// gObjects.push_back(_particle->getRenderable());

	// PhysicsEngine::getInstance().pushParticle(_particle);
//
	// _particle2 = new Particle(
	// 	physx::PxTransform(physx::PxVec3(.0f, .0f, .0f)), 
	// 	physx::PxVec3(0.0f, 0.0f, 0.0f), 
	// 	physx::PxVec3(0.0f, -9.8f, 0.0f), 
	// 	1.0f, 
	// 	Integration_Method::VERLET,
	// 	50.0f);

	// gObjects.push_back(_particle2->getRenderable());

	// PhysicsEngine::getInstance().pushParticle(_particle2);

	// _particle2->getRenderable()->setWPos(glm::vec3(_particle2->getPosition().x, _particle2->getPosition().y, _particle2->getPosition().z));
//
	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	// gObjects.push_back(new Cube(100.0, glm::vec4(1, 1, 0, 1)));

    // Material copper_material(glm::vec4(1, 1, 0, 1)); // needs to call setCopper() to initialize parameters. These parameters are used to set the uniforms at th render of the entity when called mMaterial.upload
	// copper_material.setCopper();

	// Material ruby_material;
	// ruby_material.setGold();

	// // Graphics objects (entities) of the scene

	// Sphere* s = new Sphere(50, 20, 30);
	// s->setWPos(glm::vec3(0, 0, 150));
	// s->setMaterial(ruby_material);
	// gObjects.push_back(s);


	// Sphere* copperSphere = new Sphere(50, 20, 30);
	// copperSphere->setWPos(glm::vec3(150, 0, 0));
	// copperSphere->setMaterial(copper_material);
	// gObjects.push_back(copperSphere);


}

void Scene01::update()
{
    // for (Abs_Entity* obj : gObjects) {
    //     obj->setWPos(obj->getWPos() + glm::vec3(10.0f, 10.0f, -10.5f));
    // }
	// std::cout << "Particle position: (" << _particle->getPosition().x << ", " << _particle->getPosition().y << ", " << _particle->getPosition().z << ")" << std::endl;

	// std::cout << "Particle position: (" << _particle2->getPosition().x << ", " << _particle2->getPosition().y << ", " << _particle2->getPosition().z << ")" << std::endl;

	// _particle->getRenderable()->setWPos(glm::vec3(_particle->getPosition().x, _particle->getPosition().y, _particle->getPosition().z));

	// _particle2->getRenderable()->setWPos(glm::vec3(_particle2->getPosition().x, _particle2->getPosition().y, _particle2->getPosition().z));
}
