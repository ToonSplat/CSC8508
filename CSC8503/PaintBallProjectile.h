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
			PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, const float& _impactSize, Team* _team, std::string objectName = "");
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