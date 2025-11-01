#include "Particle.h"

#include <iostream>

#include "Constants.h"
#include "ParticleGenerator.h"


// Initialize static member
uint64_t Particle::_nextId = 0;

Particle::Particle()
{
	_transform = physx::PxTransform(0, 0, 0, physx::PxQuat::PxQuat());
	_transformOriginal = physx::PxTransform(0, 0, 0, physx::PxQuat::PxQuat());
	_velocity = physx::PxVec3(0, 0, 0);
	_acceleration = physx::PxVec3(0, 0, 0);
	_damping = Constants::Physics::Damping;
	_integrationMethod = Constants::Integration_Method::VERLET;
	_color = Constants::Color::White;
	_transformPrevious.p = _transform.p;
	_transformPrevious.q = _transform.q;
	_velocityPrevious = _velocity;
	_size = Constants::Particle::Size;
	_age = 0.0;
	_speed = 0.0f;

	// init(); // ! NAO TEM INIT
}

Particle::Particle(
    const physx::PxTransform &initTransform,
    const physx::PxVec3 &initVelocity,
    const physx::PxVec3 &initAcceleration,
    double damping,
    Constants::Integration_Method integrationMethod,
    float size)
    : _velocity(initVelocity),
	_speed(initVelocity.magnitude()),
	_acceleration(initAcceleration),
	_damping(damping),
	_integrationMethod(integrationMethod),
	_color(Constants::Color::White),
	_velocityPrevious(initVelocity),
	_size(size),
	_age(0.0)
{
	_transform.p = initTransform.p;
	_transform.q = initTransform.q;
	_transformPrevious.p = initTransform.p;
	_transformPrevious.q = initTransform.q;
	_transformOriginal.p = initTransform.p;
	_transformOriginal.q = initTransform.q;

	init();
}

Particle::Particle(const physx::PxTransform& initTransform, const physx::PxVec3& initVelocity, const physx::PxVec3& initAcceleration, Constants::Integration_Method integrationMethod)
	: Particle(initTransform, initVelocity, initAcceleration, Constants::Physics::Damping, integrationMethod)
{
	init();
}

Particle::Particle(const physx::PxTransform &initTransform, const physx::PxVec3 &initVelocity, const physx::PxVec3 &initAcceleration, Constants::Integration_Method integrationMethod, float size, double damping, const physx::PxVec4 &color)
	: _velocity(initVelocity),
	  _speed(initVelocity.magnitude()),
	  _acceleration(initAcceleration),
	  _damping(damping),
	  _integrationMethod(integrationMethod),
	  _color(color),
	  _velocityPrevious(initVelocity),
	  _size(size),
	  _age(0.0)
{
	_transform.p = initTransform.p;
	_transform.q = initTransform.q;
	_transformPrevious.p = initTransform.p;
	_transformPrevious.q = initTransform.q;
	_transformOriginal.p = initTransform.p;
	_transformOriginal.q = initTransform.q;

	init();
}

Particle::Particle(float size, const physx::PxVec4 &color, float speed, Constants::Integration_Method integrationMethod)
	: Particle()
{
	_size = size;
	_color = color;
	_speed = speed;
	_velocity = physx::PxVec3(1, 0, 0).getNormalized() * _speed; // Default direction
	_integrationMethod = integrationMethod;

	init();
}

Particle::Particle(const Particle& other)
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

	init();
}

Particle::~Particle()
{
	if (_renderItem != nullptr) {
		_renderItem->release();
	}
}

void Particle::init()
{
	if (_initialized) {
		return;
	}

	_id = _nextId++;

	createRenderItem();

	deactivate();

	_firstIntegration = true;

	switch (_integrationMethod)
	{
	case Constants::Integration_Method::EULER:
		break;
	case Constants::Integration_Method::EULER_SEMI_IMPLICIT:
		break;
	case Constants::Integration_Method::VERLET:
		_velocityPrevious = _velocity;
		_transformPrevious.p = _transform.p;
		_transformPrevious.q = _transform.q;
		_transformOriginal.p = _transform.p;
		_transformOriginal.q = _transform.q;
		break;
	case Constants::Integration_Method::NONE:
		std::cout << "Particle initialized with NONE integration method. Setting velocity and acceleration to zero." << std::endl;
		_velocity = _velocityPrevious = physx::PxVec3(0, 0, 0);
		_acceleration = physx::PxVec3(0, 0, 0);
		break;
	}
}

void Particle::setTransform(const physx::PxTransform& origin)
{
	_transform.p.x = _transformPrevious.p.x = origin.p.x;
	_transform.p.y = _transformPrevious.p.y = origin.p.y;
	_transform.p.z = _transformPrevious.p.z = origin.p.z;

	_transform.q = _transformPrevious.q = origin.q;
}

void Particle::setOriginalTransform(const physx::PxTransform &transform)
{
	_transformOriginal.p = transform.p;
	_transformOriginal.q = transform.q;
}

void Particle::setTransformRelative(const physx::PxTransform &transform)
{
	_relativeTransform.p = transform.p;
	_relativeTransform.q = transform.q;
}

void Particle::setVelocity(const physx::PxVec3& velocity)
{
	_velocity = _velocityPrevious = velocity;
}

void Particle::setSpeed(float speed)
{
	_speed = speed;
	_velocity = _velocity.getNormalized() * _speed;
}

void Particle::setVelocityDirection(const physx::PxVec3 &direction)
{
	_velocity = direction.getNormalized() * _speed;
	_velocityPrevious = _velocity;
}

void Particle::createRenderItem()
{
	_shape = CreateShape(physx::PxSphereGeometry(_size));
	_renderItem = new RenderItem(_shape, &_transform, _color);
}

void Particle::activate()
{
	_alive = true;
	_age = 0.0;
	_firstIntegration = true;
	RegisterRenderItem(_renderItem);
}

void Particle::deactivate()
{
    _alive = false;
	DeregisterRenderItem(_renderItem);
}

void Particle::update(double dt) 
{
	integrate(dt);
	updateAge(dt);
}

void Particle::setColor(const physx::PxVec4 &color)
{
	_color = color;
	_renderItem->setColor(_color);
}

void Particle::setVisibility(bool visibility)
{
	_renderItem->setVisibility(visibility);
}

void Particle::setAge(double age) {
	_age = age;
}

void Particle::setAcceleration(const physx::PxVec3 &acceleration) {
	_acceleration = acceleration;
}

void Particle::setSize(double size)
{
	DeregisterRenderItem(_renderItem);
	_renderItem->release();
	_renderItem = nullptr;

	_size = size;
	createRenderItem();
	if (_alive) {
		RegisterRenderItem(_renderItem);
	}
}

void Particle::integrate(double dt)
{
	// std::cout << "Integrating: velX: " << _velocity.x << " velY: " << _velocity.y << " velZ: " << _velocity.z << '\n';

	switch (_integrationMethod) 
	{
		case Constants::Integration_Method::EULER:
			eulerIntegration(dt);
			break;
		case Constants::Integration_Method::EULER_SEMI_IMPLICIT:
			eulerSemiImplicitIntegration(dt);
			break;
		case Constants::Integration_Method::VERLET:
			verletIntegration(dt);
			break;
	}
}

void Particle::updateAge(double dt)
{
	_age += dt;
}

void Particle::eulerIntegration(double dt)
{
	// P(i+1) = P(i) + v(i)dt
	_transform.p += _velocity * dt;

	// v(i+1) = v(i) + a(i)dt
	_velocity += _acceleration * dt;

	// damping: v(i+1) = v(i+1) * d^(dt)
	_velocity *= pow(_damping, dt);
}

void Particle::eulerSemiImplicitIntegration(double dt)
{
	// First time use Euler:
	if (_firstIntegration) {
		eulerIntegration(dt);
		_firstIntegration = false;
	}

	// v(i+1) = v(i) + a(i)dt
	_velocity += _acceleration * dt;

	// damping: v(i+1) = v(i+1) * d^(dt)
	_velocity *= pow(_damping, dt);

	// P(i+1) = P(i) + v(i+1)dt
	_transform.p += _velocity * dt;
}

void Particle::verletIntegration(double dt)
{
	// First time use Euler:
	if (_firstIntegration) {
		eulerIntegration(dt);
		_firstIntegration = false;
		return;
	}

	// Se usa la velocidad en este m�todo?
	// no
	// 
	// Si no se usa, c�mo hacer el damping?
	// no, se usa la formula para la velocidad
	//
	// Est� bien inicializar la transform(t-1) como transform inicial?
	// si
	// 

	// std::cout << "Particle -> verletIntegration -> position: (" << _transform.p.x << "," << _transform.p.y << "," << _transform.p.z << ")" << std::endl;

	physx::PxVec3 a_posPrevious = _transform.p;

	_transform.p = 2 * _transform.p - _transformPrevious.p + _acceleration * pow(dt, 2);

	_transformPrevious.p = a_posPrevious;

	// The velocity is not calculated like other integrations do nor used in the position update
	// To get the velocity use getter.
	// For Verlet, the formula is:
	_velocity = (_transform.p - _transformPrevious.p) / (2 * dt);
}
