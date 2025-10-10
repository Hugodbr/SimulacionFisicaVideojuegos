#pragma once
#include <list>

class Particle;
class ParticleGenerator;

class ParticleSystem
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void init() = 0;

	// Calls integrate for each particle in list
	// Verify if has to erase a particle (life, area of interest)
	// Create new particles -> calls generator that returns a list to be incorporated by this system
	virtual void update(double dt) = 0;

protected:

	std::list<Particle*> _particleList;
	std::list<ParticleGenerator*> _generatorList;
};

