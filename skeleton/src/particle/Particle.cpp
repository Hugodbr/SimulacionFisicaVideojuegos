#include "Particle.h"

#include <iostream>

#include "Constants.h"
#include "ParticleGenerator.h"


Particle::Particle()
{
	_transform = physx::PxTransform();
	_velocity = physx::PxVec3();
	_acceleration = physx::PxVec3();
	_damping = Constants::Physics::Damping;
	_integrationMethod = Constants::Integration_Method::VERLET;
	_color = Constants::Color::White;
	_transformPrevious = _transform;
	_velocityPrevious = _velocity;
	_size = Constants::Particle::Size;
	_age = 0.0;
	_firstIntegration = true;

	init();
}

Particle::Particle(
    const physx::PxTransform &initTransform,
    const physx::PxVec3 &initVelocity,
    const physx::PxVec3 &initAcceleration,
    double damping,
    Constants::Integration_Method integrationMethod,
    float size)
    : _transform(initTransform),
      _velocity(initVelocity),
      _acceleration(initAcceleration),
      _damping(damping),
      _integrationMethod(integrationMethod),
      _color(Constants::Color::White),
      _transformPrevious(initTransform),
      _velocityPrevious(initVelocity),
      _size(size),
      _age(0.0),
      _firstIntegration(true)
{
	init();
}

Particle::Particle(const physx::PxTransform& initTransform, const physx::PxVec3& initVelocity, const physx::PxVec3& initAcceleration, Constants::Integration_Method integrationMethod)
	: Particle(initTransform, initVelocity, initAcceleration, Constants::Physics::Damping, integrationMethod)
{ }

Particle::Particle(const physx::PxTransform &initTransform, const physx::PxVec3 &initVelocity, const physx::PxVec3 &initAcceleration, Constants::Integration_Method integrationMethod, float size, double damping, const physx::PxVec4 &color)
	: _transform(initTransform),
	  _velocity(initVelocity),
	  _acceleration(initAcceleration),
	  _damping(damping),
	  _integrationMethod(integrationMethod),
	  _color(color),
	  _transformPrevious(initTransform),
	  _velocityPrevious(initVelocity),
	  _size(size),
	  _age(0.0),
	  _firstIntegration(true)
{
	init();
}

Particle::Particle(const Particle& other)
{
	_transform = other._transform;
	_velocity = other._velocity;
	_acceleration = other._acceleration;
	_damping = other._damping;
	_integrationMethod = other._integrationMethod;
	_color = other._color;
	_transformPrevious = other._transformPrevious;
	_velocityPrevious = other._velocityPrevious;
	_size = other._size;
	_age = other._age;
	_firstIntegration = true;

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
	createRenderItem();

	switch (_integrationMethod)
	{
	case Constants::Integration_Method::EULER:
		break;
	case Constants::Integration_Method::EULER_SEMI_IMPLICIT:
		break;
	case Constants::Integration_Method::VERLET:
		_velocityPrevious = _velocity;
		_transformPrevious = _transform;
		break;
	case Constants::Integration_Method::NONE:
		_velocity = _velocityPrevious = physx::PxVec3(0, 0, 0);
		_acceleration = physx::PxVec3(0, 0, 0);
		break;
	}
}

std::unique_ptr<Particle> Particle::clone() const
{
	return std::make_unique<Particle>(*this);
}

void Particle::setOrigin(const physx::PxTransform& origin)
{
	_transform.p = _transformPrevious.p = origin.p;	
}

void Particle::setVelocity(const physx::PxVec3& velocity)
{
	_velocity = _velocityPrevious = velocity;
}

void Particle::createRenderItem()
{
	_shape = CreateShape(physx::PxSphereGeometry(_size));
	_renderItem = new RenderItem(_shape, &_transform, _color);
}

double Particle::getAge() const {
	return _age;
}

physx::PxVec3 Particle::getPosition() const {
	return _transform.p;
}

physx::PxVec3 Particle::getVelocity() const {
	return _velocity;
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

void Particle::integrate(double dt)
{
	//std::cout << "Integrating: velX: " << _velocity.x << " velY: " << _velocity.y << " velZ: " << _velocity.z << '\n';

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

	//std::cout << "Particle -> verletIntegration -> position: (" << _transform.p.x << "," << _transform.p.y << "," << _transform.p.z << ")" << std::endl;

	physx::PxTransform a_posPrevious = _transform;

	_transform.p = 2 * _transform.p - _transformPrevious.p + _acceleration * pow(dt, 2);

	_transformPrevious = a_posPrevious;

	// The velocity is not calculated like other integrations do nor used in the position update
	// To get the velocity use getter.
	// For Verlet, the formula is:
	_velocity = (_transform.p - _transformPrevious.p) / (2 * dt);
}
