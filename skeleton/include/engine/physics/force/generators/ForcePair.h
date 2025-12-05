#pragma once

#include "ForceGenerator.h"

class ParticleWithMass;

class SpringForce : public virtual ForceGenerator 
{
public:
    SpringForce(ParticleWithMass* mainParticle, double k, double restingLength);
    virtual ~SpringForce();

    virtual void updateForce(double deltaTime) override;
    virtual void setK(double k) { _k = k; };
    virtual void setOtherParticle(ParticleWithMass* otherParticle);

private:
    double _k;
    double _restingLength;

    ParticleWithMass* _mainParticle;
    ParticleWithMass* _otherParticle;
};
