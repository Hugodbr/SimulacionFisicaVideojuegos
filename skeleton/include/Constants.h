#pragma once

#include <PxPhysicsAPI.h>

// Sistema internacional

namespace Constants
{
	namespace Math {
		static constexpr float epsilon = 1e-6f;
	}

	namespace Physics {
		static const physx::PxVec3 Gravity = physx::PxVec3(0, -9.8f, 0);
		static const double GravitationalConstant = 6.67430e-11; // in m^3 kg^-1 s^-2
		static constexpr float EarthMass = 5.972e24f; // in kg
		static constexpr float EarthRadius = 6.371e6f; // in meters

		static constexpr float Damping = 0.98f;
	};

	enum ParticleType {
		DEFAULT,
		CANNON_BALL,
		BULLET,
		RAIN
	};

	enum Integration_Method {
		EULER,
		EULER_SEMI_IMPLICIT,
		VERLET,
		NONE
	};

	namespace Particle {

		static constexpr float Size = 0.5f;

		namespace Default {
			static constexpr double gFactor = 0.5;
			static constexpr double vFactor = 0.5;
		};

		namespace WithMass {

			namespace Default {
				static const ParticleType type = DEFAULT;

			};

			namespace CannonBall {
				static const ParticleType type = CANNON_BALL;
				static constexpr float Size = 5.0f;
				static constexpr float Speed = 500.0f;
				static constexpr float Mass = 5.0f;
				static constexpr double gFactor = 0.5;
				static constexpr double vFactor = 0.1;

			};

			namespace Bullet {
				static const ParticleType type = BULLET;
				static constexpr float Size = 0.5f;
				static constexpr float Speed = 1000.0f;
				static constexpr float Mass = 0.001f;
				static constexpr double gFactor = 0.1;
				static constexpr double vFactor = 1;
			};

			namespace Rain {
				static const ParticleType type = RAIN;
				static constexpr float Size = 0.1f;
				static constexpr float Speed = 0.0f;
				static constexpr float Mass = 1.0f;
				static constexpr double gFactor = 0;
				static constexpr double vFactor = 0;
			};
		};


	};

	namespace System {

		static constexpr uint32_t MaxInsideForceGenerators = 10;
		static constexpr uint32_t MaxSubSystems = 10;
		namespace Default {
			static constexpr uint64_t ReserveCountPerGenerator = 1000;
		};
		namespace Grid {
			static constexpr uint64_t ReserveCountPerGenerator = 10000;
		};
		namespace Rain {
			static constexpr uint64_t ReserveCountPerGenerator = 15000;
		};

		namespace Gun {
			static constexpr uint64_t ReserveCountPerGenerator = 254;
			static constexpr uint64_t ReserveCountPerBulletGenerator = 30;
		};
	};

	namespace Color {
		static const physx::PxVec4 White = physx::PxVec4(255, 255, 255, 255);
		static const physx::PxVec4 Red   = physx::PxVec4(255, 0, 0, 255);
		static const physx::PxVec4 Green = physx::PxVec4(0, 255, 0, 255);
		static const physx::PxVec4 Blue  = physx::PxVec4(0, 0, 255, 255);
	};

};

