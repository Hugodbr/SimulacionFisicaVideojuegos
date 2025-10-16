#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>


class Particle;

struct Vector3Stats {
	physx::PxVec3 mean;
	physx::PxVec3 deviation;
};

struct ScalarStats {
	double mean;
	double deviation;
};

struct ParticleLifetimePolicy {
	bool useLifetime;        // expire after time
	bool useVolumeBounds;    // expire when out of region
	bool useCustomCondition; // maybe user-defined callback

	double maxLifetime;       // seconds
	physx::PxBounds3 bounds;  // spatial limits (min/max positions)
};

// Abstract class
// The Generator describes how to spawn and what the death rules are
class ParticleGenerator
{
public:

	ParticleGenerator();
	virtual ~ParticleGenerator();

	virtual void init(
		const Vector3Stats& position, 
		const Vector3Stats& velocity, 
		const ScalarStats lifetime, 
		const Particle* modelParticle
	) = 0;

	virtual std::list<Particle*> generateParticles() = 0;

	// Setters
	virtual void setMeanPosition(const physx::PxVec3& pos) = 0;
	virtual void setPositionDeviation(const physx::PxVec3& dev) = 0;

	virtual void setMeanVelocity(const physx::PxVec3& vel) = 0;
	virtual void setVelocityDeviation(const physx::PxVec3& dev) = 0;

	virtual void setMeanLifetime(double mean) = 0;
	virtual void setLifetimeDeviation(double dev) = 0;

	virtual void setSpawnProbability(double probability) = 0;
	virtual void setSpawnCount(int count) = 0;

	virtual void setModelParticle(const Particle* model) = 0;

	// Getters
	virtual physx::PxVec3 getMeanPosition() const;
	virtual physx::PxVec3 getMeanVelocity() const;
	virtual double getMeanLifetime() const;

	virtual physx::PxVec3 getPositionDeviation() const;
	virtual physx::PxVec3 getVelocityDeviation() const;
	virtual double getLifetimeDeviation() const;

	virtual double getSpawnProbability() const;
	virtual int getSpawnCount() const;

	virtual const Particle& getModelParticle() const;


protected:

	std::mt19937 _mt; // Random. Produces deterministic numbers

	const Particle* _modelParticle;

	physx::PxVec3 _meanPosition;
	physx::PxVec3 _meanVelocity;
	double _meanLifetime;

	physx::PxVec3 _positionDeviation;
	physx::PxVec3 _velocityDeviation;
	double _lifetimeDeviation;

	double _spawnProbability; // probability of spawning any particle at all at a frame
	int _spawnCount; // how many particles are spawning when they do spawn
};

