#pragma once

#include <PxPhysicsAPI.h>

// Sistema internacional

namespace Constants
{
	namespace Math {
		static inline constexpr float epsilon = 1e-6f;
	}

	namespace Physics {
		static inline const physx::PxVec3 Gravity = physx::PxVec3(0, -9.81f, 0);
		static inline const double GravitationalConstant = 6.67430e-11; // in m^3 kg^-1 s^-2
		static inline constexpr float EarthMass = 5.972e24f; // in kg
		static inline constexpr float EarthRadius = 6.371e6f; // in meters

		static inline constexpr float Damping = 0.98f;
	};

	// Group that systems or forces can belong to. Used for selective application of forces.
	namespace Group {
		enum DynamicGroup {
			ALL = 0,    // Applies to all systems
			NONE = 1,   // Applies to no systems
			ENEMY = 2,   // Applies to enemy systems
			ENVIRONMENT = 3, // Applies to environment systems
			// TODO
		};
	}

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

	namespace Color {
		static inline const physx::PxVec4 White = physx::PxVec4(1.0f, 1.0f, 1.0f, 1.0f);
		static inline const physx::PxVec4 Gray  = physx::PxVec4(0.5f, 0.5f, 0.5f, 1.0f);
		static inline const physx::PxVec4 Black = physx::PxVec4(0.0f, 0.0f, 0.0f, 1.0f);

		static inline const physx::PxVec4 Red   = physx::PxVec4(1.0f, 0.0f, 0.0f, 1.0f);
		static inline const physx::PxVec4 Yellow = physx::PxVec4(1.0f, 1.0f, 0.0f, 1.0f);
		static inline const physx::PxVec4 Green = physx::PxVec4(0.0f, 1.0f, 0.0f, 1.0f);
		static inline const physx::PxVec4 Cyan  = physx::PxVec4(0.0f, 1.0f, 1.0f, 1.0f);
		static inline const physx::PxVec4 Blue  = physx::PxVec4(0.0f, 0.0f, 1.0f, 1.0f);
		static inline const physx::PxVec4 Magenta = physx::PxVec4(1.0f, 0.0f, 1.0f, 1.0f);
		static inline const physx::PxVec4 Orange = physx::PxVec4(1.0f, 0.647f, 0.0f, 1.0f);
		static inline const physx::PxVec4 Purple = physx::PxVec4(0.5f, 0.0f, 0.5f, 1.0f);
		static inline const physx::PxVec4 Pink = physx::PxVec4(1.0f, 0.753f, 0.796f, 1.0f);
		static inline const physx::PxVec4 Brown = physx::PxVec4(0.647f, 0.165f, 0.165f, 1.0f);
	};

	namespace Particle {


		namespace Default {
			static inline constexpr double gFactor = 0.5;
			static inline constexpr double vFactor = 0.5;
			static inline constexpr float Size = 0.5f;

		};

		namespace WithMass {

			namespace Default {
				static inline const ParticleType type = DEFAULT;

			};

			namespace CannonBall {
				static inline const ParticleType type = CANNON_BALL;
				static inline constexpr float Size = 5.0f;
				static inline constexpr float Speed = 500.0f;
				static inline constexpr float Mass = 5.0f;
				static inline constexpr double gFactor = 0.5;
				static inline constexpr double vFactor = 0.1;
				static inline const physx::PxVec4 Color = Color::Gray;
			};

			namespace Bullet {
				static inline const ParticleType type = BULLET;
				static inline constexpr float Size = 3.0f;
				static inline constexpr float Speed = 2000.0f;
				static inline constexpr float Mass = 0.001f;
				static inline constexpr double gFactor = 0.1;
				static inline constexpr double vFactor = 1;
				static inline const physx::PxVec4 Color = Color::Yellow;
			};

			namespace Rain {
				static inline const ParticleType type = RAIN;
				static inline constexpr float Size = 0.1f;
				static inline constexpr float Speed = 0.0f;
				static inline constexpr float Mass = 1.0f;
				static inline constexpr double gFactor = 0;
				static inline constexpr double vFactor = 0;
				static inline const physx::PxVec4 Color = Color::White;
			};
		};


	};

	namespace System {
		static inline constexpr size_t MaxParticleSystems = 50;
		static inline constexpr size_t MaxInsideForceGenerators = 10;
		static inline constexpr size_t MaxSubSystems = 10;
		namespace Default {
			static inline constexpr uint64_t ReserveCountPerGenerator = 1000;
		};
		namespace Grid {
			static inline constexpr uint64_t ReserveCountPerGenerator = 10000;
		};
		namespace Rain {
			static inline constexpr uint64_t ReserveCountPerGenerator = 10000;
		};
		namespace Fog {
			static inline constexpr uint64_t ReserveCountPerGenerator = 10000;
		};
		namespace Splash {
			static inline constexpr uint64_t ReserveCountPerGenerator = 1000;
		};
		namespace Gun {
			static inline constexpr uint64_t ReserveCountPerBulletGenerator = 30;
			static inline constexpr uint64_t ReserveCountPerMuzzleGenerator = 3000;
		};

		namespace Trash {
			static inline constexpr uint64_t ReserveCountPerGenerator = 3;
		};
		namespace Rope {
			static inline constexpr uint64_t ReserveCountPerGenerator = 50;
		};
		namespace Spring {
			static inline constexpr uint64_t ReserveCountPerGenerator = 40;
		};

	};
};

