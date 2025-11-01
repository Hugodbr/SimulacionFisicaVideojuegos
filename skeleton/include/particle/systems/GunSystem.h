#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"

class ConstantParticleGenerator;
class UniformParticleGenerator;
class GaussianParticleGenerator;
class StaticParticle;
class Bullet;
class MeshData;

class GunSystem : public ParticleSystem
{
private:
	physx::PxBounds3 _region;
	
public:

	GunSystem(const physx::PxVec3 &position, Camera* cam);
	~GunSystem() = default;

	void init() override;
	void update(double deltaTime) override;

    void setTransform(const physx::PxTransform& t);

	void shoot();

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
		return Constants::System::Gun::ReserveCountPerGenerator; // TODO change to mesh vertices
	}

protected:

    void initParticleGeneratorAndPool();
	void initGunMesh();
	void initGunBullets();

    void createGun();
	void createMuzzleFlash(double deltaTime);

	void updateGunMesh(double deltaTime);
	void updateGunBullets(double deltaTime);
	void updateMuzzleFlash(double deltaTime);

    Camera* _camera;

	std::vector<MeshData> _meshDataGun;
	std::vector<MeshData> _meshDataMuzzle;

	physx::PxTransform _gunTransform;

	int currentAmmunition;
	bool isShooting;

	// The idea is to have flexibility to have multiple generators and pools if needed for mesh composition
	std::vector<std::pair<
		std::unique_ptr<ConstantParticleGenerator>,
		std::unique_ptr<ParticlePool<StaticParticle>>
            >> _gunMeshGeneratorAndPool;

	// Muzzle flash generator and pool (could be multiple types of muzzle flashes)
	std::vector<std::pair<
		std::unique_ptr<GaussianParticleGenerator>,
		std::unique_ptr<ParticlePool<Particle>>
            >> _gunMuzzleGeneratorAndPool;

	// Also flexible to have multiple bullet generators and pools (different bullet types)
	std::vector<std::pair<
		std::unique_ptr<ConstantParticleGenerator>,
		std::unique_ptr<ParticlePool<Bullet>>
			>> _gunBulletsGeneratorAndPool;
};