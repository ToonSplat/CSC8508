#pragma once
#include "Vector3.h"
#include "Team.h"

namespace NCL {
	namespace CSC8503 {
		class ImpactPoint {
		public:
			ImpactPoint(Vector3 location, Team* team, float radius) : location(location), team(team), radius(radius) {}

			int GetTeamID() { return team->GetTeamID(); }

			Vector3 GetImpactLocation() const { return location; }
			void SetImpactLocation(NCL::Maths::Vector3 localPoint) { location = localPoint; }

			Vector3 GetImpactColour() { return team->GetTeamColour(); }

			float GetImpactRadius() const { return radius; }
		protected:
			NCL::Maths::Vector3 location;
			Team* team = nullptr;
			float radius;
		};
	}
}