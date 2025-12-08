#include "Particle.h"

#include <iostream>

#include "Constants.h"
#include "PBRMaterial.h"


// Initialize static member
uint64_t Particle::_nextId = 0;

Particle::Particle(
    const physx::PxTransform &initTransform,
    const physx::PxVec3 &initVelocity,
    double damping,
    Constants::Integration_Method integrationMethod,
    float size
)
	: PhysicalObject(
		initTransform,
		initVelocity,
		damping,
		integrationMethod,
		size
	)
{
	_transform.p = initTransform.p;
	_transform.q = initTransform.q;
	_transformPrevious.p = initTransform.p;
	_transformPrevious.q = initTransform.q;
	_transformOriginal.p = initTransform.p;
	_transformOriginal.q = initTransform.q;
}

Particle::Particle(
    float size,
    float speed,
    Constants::Integration_Method integrationMethod
) 
	: PhysicalObject(
		physx::PxTransform(physx::PxVec3(0, 0, 0)),
		physx::PxVec3(1, 0, 0).getNormalized() * speed,
		Constants::Physics::Damping,
		integrationMethod,
		size
	)
{
	_size = size;
	_speed = speed;
	_integrationMethod = integrationMethod;
	_color = Constants::Color::White;
	_age = 0.0;
}

Particle::Particle(
	float size, 
	const physx::PxVec4 &color, 
	float speed, 
	Constants::Integration_Method integrationMethod
)
	: PhysicalObject(
		physx::PxTransform(physx::PxVec3(0, 0, 0)),
		physx::PxVec3(1, 0, 0).getNormalized() * speed,
		Constants::Physics::Damping,
		integrationMethod,
		size
	)
{
}

Particle::Particle(const Particle& other)
	: PhysicalObject(other) // Call base class copy constructor
	, _id() // New ID will be assigned in onInit()
{
	_transform.p = other._transform.p;
	_transform.q = other._transform.q;
	_transformOriginal.p = other._transformOriginal.p;
	_transformOriginal.q = other._transformOriginal.q;
	_velocity = other._velocity;
	_acceleration = other._acceleration;
	_damping = other._damping;
	_integrationMethod = other._integrationMethod;
	_color = other._color;
	_transformPrevious.p = other._transformPrevious.p;
	_transformPrevious.q = other._transformPrevious.q;
	_velocityPrevious = other._velocityPrevious;
	_size = other._size;
	_age = other._age;
	_speed = other._speed;
}

Particle::~Particle()
{
}

void Particle::onInit()
{
	_id = _nextId++;
	deactivate();
}

// void Particle::createRender()
// {
// 	// _shape = createShape(physx::PxSphereGeometry(_size));

// 	// _renderable = new Sphere(_size, 30, 30, glm::vec4(_color.x, _color.y, _color.z, _color.w));
// 	// _renderable = new RGBCube(_size);
// 	// _renderable = new Toros(_size, _size/3, 50, 40, glm::vec4(_color.x, _color.y, _color.z, _color.w));
// 	// _renderable = new IndexedBox(_size*10, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
// 	// _renderable = new IndexedMesh("C:\\Users\\hugod\\Github\\PhysxIG1\\resources\\blender\\untitled.obj", glm::vec4(_color.x, _color.y, _color.z, _color.w));
// 	// _renderable = new ModelEntity("C:\\Users\\hugod\\Github\\PhysxIG1\\resources\\fbx\\smily-horror-monster\\source\\monster.fbx", 90.0f);
// 	// _renderable = new ModelEntity("C:\\Users\\hugod\\Github\\PhysxIG1\\resources\\fbx\\bloody-bathtub-game-ready\\source\\model.dae", 2.0f);

// 	// _renderable = new ModelEntity("C:\\Users\\hugod\\Github\\PhysxIG1\\resources\\fbx\\nurse-of-horror\\source\\nurse_lp.fbx", 50.0f);

// 	// _renderable = new IndexedBox(_size, glm::vec4(_color.x, _color.y, _color.z, _color.w));
// 	// _renderable = new Sphere(_size, 10, 10, glm::vec4(_color.x, _color.y, _color.z, _color.w));

// 	// _renderable = new Sphere(_size, 10, 10, glm::vec4(_color.x, _color.y, _color.z, _color.w));
// 	// Material mat;
// 	// mat.setGold();
// 	// _renderable = new ModelSingleMeshMaterial("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\blender\\monkey.obj");
// 	// _renderable = new ModelSingleMeshPBR("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\blender\\rainDropGLB.glb");
// 	// _renderable = new ModelSingleMeshPBR("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\fbx\\crate-box\\source\\796b3b0283794320bc74bbccf8578115.fbx.fbx");
// 	_renderable = new ModelSingleMeshPBR("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\fbx\\crate-box-free\\source\\Crate.fbx");
// 	// _renderable = new ModelSingleMeshPBR("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\fbx\\smily-horror-monster\\source\\monster.fbx");

// 	// static_cast<ModelSingleMeshMaterial*>(_renderable)->setMaterial(mat);
// 	_renderable->setWPos(glm::vec3(_transform.p.x, _transform.p.y, _transform.p.z));
// }