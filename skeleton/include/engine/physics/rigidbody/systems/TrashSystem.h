#pragma once

#include "RigidBodySystem.h"
#include "Region.h"
#include "GaussianRBGenerator.h"
#include "RigidBodyPool.h"
#include "BoxBody.h"

class ForceGenerator;

class TrashSystem : public RigidBodySystem
{
private:
	Region _region;
	
public:
	TrashSystem(const physx::PxVec3 &origin, const Region& region, const std::string& filePath, float scale = 1.0f);
	~TrashSystem() = default;

	void init() override;
	void update(double deltaTime) override;

    virtual void render(const glm::mat4& modelViewMat) override;

    void setFollowTarget(PhysicalObject& target);

protected:
    virtual void initRigidBodyGeneratorAndPool(const std::string& filePath, float scale);
	virtual void applyForces() override;

protected:
    std::pair<
        std::unique_ptr<GaussianRigidBodyGenerator>,
        std::unique_ptr<RigidBodyPool<BoxBody>>
            >_generatorAndPool;

    physx::PxVec3 _origin;
    PhysicalObject* _followTarget = nullptr;
};
