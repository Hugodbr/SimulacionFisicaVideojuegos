#include "Particle.h"

#include <iostream>

Particle::Particle(const physx::PxTransform& pose, const  physx::PxVec3& velocity, const physx::PxVec3& acceleration, double damping, Constants::Integration_Method integrationMethod)
	:
	_pose(pose),
	_vel(velocity),
	_acc(acceleration),
	_damping(damping),
	_integrationMethod(integrationMethod),
	_renderItem(nullptr),
	_shape(nullptr),
	_color(physx::PxVec4(255, 255, 255, 255)),
	_posePrevious(pose),
	_velPrevious(velocity),
	_gravity(0.0),
	_mass(0.0),
	_size(0.5),
	_lifeTime(0.0),
	_firstIntegration(true)
{
	createRenderItem();

	switch (_integrationMethod) {
	case Constants::Integration_Method::EULER:
			break;
	case Constants::Integration_Method::EULER_SEMI_IMPLICIT:
			break;
	case Constants::Integration_Method::VERLET:
			_velPrevious = _vel;
			_posePrevious = _pose;
			break;
	}
}

Particle::Particle(const physx::PxTransform& pose, const physx::PxVec3& velocity, const physx::PxVec3& acceleration, double mass, double damping, Constants::Integration_Method integrationMethod)
	: Particle(pose, velocity, acceleration, damping, integrationMethod)
{
	_massReal = mass;
}

Particle::Particle(const physx::PxTransform& pose, const physx::PxVec3& velocity, const physx::PxVec3& acceleration, const physx::PxVec3 &gravity, double mass, double damping, Constants::Integration_Method integrationMethod)
	: Particle(pose, velocity, acceleration, damping, integrationMethod)
{
	_massReal = mass;
	_gravityReal = gravity;
}

Particle::~Particle()
{
	DeregisterRenderItem(_renderItem);
}

void Particle::createRenderItem()
{
	_shape = CreateShape(physx::PxSphereGeometry(_size));
	_renderItem = new RenderItem(_shape, &_pose, _color);
	RegisterRenderItem(_renderItem);
}

const physx::PxVec3& Particle::getVelocity() const
{
	return _vel;
}

void Particle::integrate(double dt)
{
	std::cout << "integrate: " << _vel.y << '\n';

	switch (_integrationMethod) {
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

void Particle::eulerIntegration(double dt)
{
	// P(i+1) = P(i) + v(i)dt
	_pose.p += _vel * dt;

	// v(i+1) = v(i) + a(i)dt
	_vel += _acc * dt;

	// damping: v(i+1) = v(i+1) * d^(dt)
	_vel *= pow(_damping, dt);
}

void Particle::eulerSemiImplicitIntegration(double dt)
{
	// First time use Euler:
	if (_firstIntegration) {
		eulerIntegration(dt);
		_firstIntegration = false;
	}

	// v(i+1) = v(i) + a(i)dt
	_vel += _acc * dt;

	// damping: v(i+1) = v(i+1) * d^(dt)
	_vel *= pow(_damping, dt);

	// P(i+1) = P(i) + v(i+1)dt
	_pose.p += _vel * dt;
}

void Particle::verletIntegration(double dt)
{
	// Se usa la velocidad en este método?
	// no
	// 
	// Si no se usa, cómo hacer el damping?
	// no, se usa la formula para la velocidad
	//
	// Está bien inicializar la pose(t-1) como pose inicial?
	// si
	// 

	const physx::PxTransform a_posPrevious = _pose;

	_pose.p = 2 * _pose.p - _posePrevious.p + _acc * pow(dt, 2);

	_posePrevious = a_posPrevious;

	// The velocity is not calculated like other integrations do nor used in the position update
	// To get the velocity use getter.
	// For Verlet, the formula is:
	_vel = (_pose.p - _posePrevious.p) / 2 * dt;
}

void Particle::setSimulatedMass()
{
}

void Particle::setSimulatedVelocity()
{
}

void Particle::setSimulatedGravity()
{
}

void Particle::setSimulatedAcceleration()
{
}
