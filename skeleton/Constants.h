#pragma once

// Sistema internacional

namespace Constants
{
	namespace Physics {
		static const physx::PxVec3 Gravity = physx::PxVec3(0, -9.8f, 0);
		static constexpr float Damping = 0.98f;
	};

	enum ParticleType {
		DEFAULT,
		CANNON_BALL,
		BULLET,
	};

	enum Integration_Method {
		EULER,
		EULER_SEMI_IMPLICIT,
		VERLET
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
				static const float Speed = 500.0f;
				static constexpr float Mass = 5.0f;
				static constexpr double gFactor = 0.5;
				static constexpr double vFactor = 0.1;

			};

			namespace Bullet {
				static const ParticleType type = BULLET;
				static constexpr float Size = 0.5f;
				static const float Speed = 1000.0f;
				static constexpr float Mass = 0.001f;
				static constexpr double gFactor = 0.1;
				static constexpr double vFactor = 1;
			};
		};


	};

	namespace Color {
		static const physx::PxVec4 White = physx::PxVec4(255, 255, 255, 255);
		static const physx::PxVec4 Red   = physx::PxVec4(255, 0, 0, 255);
		static const physx::PxVec4 Green = physx::PxVec4(0, 255, 0, 255);
		static const physx::PxVec4 Blue  = physx::PxVec4(0, 0, 255, 255);
	};

};

