#include "Particle.h"

#include <iostream>

#include "Constants.h"
#include "ParticleGenerator.h"


Particle::Particle(
	const physx::PxTransform& initTransform, 
	const physx::PxVec3& initVelocity, 
	const physx::PxVec3& initAcceleration, 
	double damping, 
	Constants::Integration_Method integrationMethod,
	float size
)
	:
	_transform(initTransform),
	_velocity(initVelocity),
	_acceleration(initAcceleration),
	_damping(damping),
	_integrationMethod(integrationMethod),
	_renderItem(nullptr),
	_shape(nullptr),
	_color(Constants::Color::White),
	_transformPrevious(initTransform),
	_velocityPrevious(initVelocity),
	_size(size),
	_age(0.0),
	_alive(true),
	_firstIntegration(true)
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
	}
}

Particle::Particle(const physx::PxTransform& initTransform, const physx::PxVec3& initVelocity, const physx::PxVec3& initAcceleration, Constants::Integration_Method integrationMethod)
	:Particle(initTransform, initVelocity, initAcceleration, Constants::Physics::Damping, integrationMethod)
{
}

Particle::~Particle()
{
	DeregisterRenderItem(_renderItem);
}

std::unique_ptr<Particle> Particle::clone() const
{
	return std::make_unique<Particle>(*this);
}

void Particle::setOrigin(const physx::PxTransform& origin)
{
	_transform = _transformPrevious = origin;	
}

void Particle::setVelocity(const physx::PxVec3& velocity)
{
	_velocity = _velocityPrevious = velocity;
}

void Particle::createRenderItem()
{
	_shape = CreateShape(physx::PxSphereGeometry(_size));
	_renderItem = new RenderItem(_shape, &_transform, _color);
	RegisterRenderItem(_renderItem);
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

void Particle::setAge(double age) {
	_age = age;
}

void Particle::integrate(double dt)
{
	std::cout << "Integrating: velX: " << _velocity.x << " velY: " << _velocity.y << " velZ: " << _velocity.z << '\n';

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

//bool Particle::isAlive() const
//{
//	return _alive;
//}
//
//void Particle::kill()
//{
//	_alive = false;
//}

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

	// Se usa la velocidad en este método?
	// no
	// 
	// Si no se usa, cómo hacer el damping?
	// no, se usa la formula para la velocidad
	//
	// Está bien inicializar la transform(t-1) como transform inicial?
	// si
	// 

	physx::PxTransform a_posPrevious = _transform;

	_transform.p = 2 * _transform.p - _transformPrevious.p + _acceleration * pow(dt, 2);

	_transformPrevious = a_posPrevious;

	// The velocity is not calculated like other integrations do nor used in the position update
	// To get the velocity use getter.
	// For Verlet, the formula is:
	_velocity = (_transform.p - _transformPrevious.p) / (2 * dt);
}
