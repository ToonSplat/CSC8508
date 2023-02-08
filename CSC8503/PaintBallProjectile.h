#pragma once
#include "ToonGameObject.h"
#include "Team.h"
#include "AudioSystem.h"

namespace NCL 
{
	namespace CSC8503
	{
		class PaintBallProjectile : public ToonGameObject 
		{
		public:
			PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const reactphysics3d::Vector3& position,
				const reactphysics3d::Vector3& rotationEuler, const float& radius, const float& _impactSize, Team* _team);
			~PaintBallProjectile();

			float GetImpactSize() const { return impactSize; }
			Team* GetTeam() const { return team; }

			AudioEmitter* GetAudioEmitter() { return speaker; }
			

		protected:
			
			Team* team;
			float impactSize;

			AudioEmitter* speaker;
			void ConfigureSound();
		};
	}
}