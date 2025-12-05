#include "PhysicalObject.h"

#include <iostream>

#include "Constants.h"


// Initialize static member
uint64_t PhysicalObject::_nextId = 0;

PhysicalObject::PhysicalObject(
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

PhysicalObject::PhysicalObject(const physx::PxTransform& initTransform, const physx::PxVec3& initVelocity, const physx::PxVec3& initAcceleration, Constants::Integration_Method integrationMethod)
	: PhysicalObject(initTransform, initVelocity, initAcceleration, Constants::Physics::Damping, integrationMethod)
{
	init();
}

PhysicalObject::PhysicalObject(const physx::PxTransform &initTransform, const physx::PxVec3 &initVelocity, const physx::PxVec3 &initAcceleration, float size, double damping, const physx::PxVec4 &color, Constants::Integration_Method integrationMethod)
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

PhysicalObject::PhysicalObject(float size, const physx::PxVec4 &color, float speed, Constants::Integration_Method integrationMethod)
	: PhysicalObject(physx::PxTransform(physx::PxVec3(0,0,0)), physx::PxVec3(1,0,0).getNormalized() * speed, physx::PxVec3(0,0,0), Constants::Physics::Damping, integrationMethod, size)
{
	_size = size;
	_color = color;
	_speed = speed;
	_integrationMethod = integrationMethod;

	init();
}

PhysicalObject::PhysicalObject(const PhysicalObject& other)
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

	init();
}

PhysicalObject::~PhysicalObject()
{
	// if (_renderItem != nullptr) {
	// 	_renderItem->release();
	// }
}

void PhysicalObject::init()
{
	if (_initialized) {
		return;
	}

	_id = _nextId++;

	// createRenderItem();

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
		// std::cout << "PhysicalObject initialized with NONE integration method. Setting velocity and acceleration to zero." << std::endl;
		_velocity = _velocityPrevious = physx::PxVec3(0, 0, 0);
		_acceleration = physx::PxVec3(0, 0, 0);
		break;
	}
}

void PhysicalObject::setTransform(const physx::PxTransform& origin)
{
	_transform.p.x = _transformPrevious.p.x = origin.p.x;
	_transform.p.y = _transformPrevious.p.y = origin.p.y;
	_transform.p.z = _transformPrevious.p.z = origin.p.z;

	_transform.q = _transformPrevious.q = origin.q;
}

void PhysicalObject::setOriginalTransform(const physx::PxTransform &transform)
{
	_transformOriginal.p = transform.p;
	_transformOriginal.q = transform.q;
}

void PhysicalObject::setTransformRelative(const physx::PxTransform &transform)
{
	_relativeTransform.p = transform.p;
	_relativeTransform.q = transform.q;
}

void PhysicalObject::setVelocity(const physx::PxVec3& velocity)
{
	_velocity = _velocityPrevious = velocity;
}

void PhysicalObject::setSpeed(float speed)
{
	_speed = speed;
	_velocity = _velocity.getNormalized() * _speed;
}

void PhysicalObject::setVelocityDirection(const physx::PxVec3 &direction)
{
	_velocity = direction.getNormalized() * _speed;
	_velocityPrevious = _velocity;
}

// void PhysicalObject::createRenderItem()
// {
// 	_shape = CreateShape(physx::PxSphereGeometry(_size));
// 	_renderItem = new RenderItem(_shape, &_transform, _color);
// }

void PhysicalObject::activate()
{
	_alive = true;
	// _renderItem->setVisibility(true);
	_age = 0.0;
	_firstIntegration = true;
}

void PhysicalObject::deactivate()
{
    _alive = false;
	// _renderItem->setVisibility(false);
}

void PhysicalObject::update(double dt) 
{
	integrate(dt);
	updateAge(dt);
}

void PhysicalObject::setColor(const physx::PxVec4 &color)
{
	_color = color;
	// _renderItem->setColor(_color);
}

// void PhysicalObject::setVisibility(bool visibility)
// {
// 	_renderItem->setVisibility(visibility);
// }

void PhysicalObject::setAge(double age) {
	_age = age;
}

void PhysicalObject::setAcceleration(const physx::PxVec3 &acceleration) {
	_acceleration = acceleration;
}

void PhysicalObject::setSize(double size)
{
	_size = size;
	// _renderItem->changeSize(static_cast<float>(size));
	// createRenderItem();

	// _renderItem->it
	// if (_alive) {
	// 	RegisterRenderItem(_renderItem);
	// }
}

void PhysicalObject::integrate(double dt)
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

void PhysicalObject::updateAge(double dt)
{
	_age += dt;
}

void PhysicalObject::eulerIntegration(double dt)
{
	// P(i+1) = P(i) + v(i)dt
	_transform.p += _velocity * dt;

	// v(i+1) = v(i) + a(i)dt
	_velocity += _acceleration * dt;

	// damping: v(i+1) = v(i+1) * d^(dt)
	_velocity *= pow(_damping, dt);
}

void PhysicalObject::eulerSemiImplicitIntegration(double dt)
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

void PhysicalObject::verletIntegration(double dt)
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

	// std::cout << "PhysicalObject -> verletIntegration -> position: (" << _transform.p.x << "," << _transform.p.y << "," << _transform.p.z << ")" << std::endl;

	physx::PxVec3 a_posPrevious = _transform.p;

	_transform.p = 2 * _transform.p - _transformPrevious.p + _acceleration * pow(dt, 2);

	_transformPrevious.p = a_posPrevious;

	// The velocity is not calculated like other integrations do nor used in the position update
	// To get the velocity use getter.
	// For Verlet, the formula is:
	_velocity = (_transform.p - _transformPrevious.p) / (2 * dt);
}
