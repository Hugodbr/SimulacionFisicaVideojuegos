#pragma once

#include <list>
#include <assert.h>

#include "PxPhysicsAPI.h"

#include "ObjectSystem.h"

class ParticleGenerator;
class Particle;

// Abstract class
// The System applies the generation rules during the update
class ParticleSystem : public ObjectSystem
{
public:
	ParticleSystem();
	virtual ~ParticleSystem() = default;

	virtual void onRender(const glm::mat4& modelViewMat) = 0;

	void setRenderableForParticle(Particle &particle);

	bool mustKillParticle(const Particle &p, const ParticleGenerator &generator) const;
	bool mustSpawnParticle(double deltaTime, const ParticleGenerator &generator) const;

	virtual void setEmissionRate(int spawnMeanRate) {};
	virtual int getEmissionRate() const;

protected:
	virtual bool doForceAffectsSystem(const ForceGenerator& forceGen) const override;
};
