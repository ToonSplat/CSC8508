#pragma once
#include <reactphysics3d/reactphysics3d.h>

namespace NCL {
	namespace CSC8503 {
		class ToonObjectState {
		public:
			ToonObjectState() {
				stateID = 0;
			}

			~ToonObjectState() {
			}

			// TODO: Replace with shorts, Vecs/Quats use floating points and are big!

			reactphysics3d::Vector3		position;
			reactphysics3d::Quaternion	orientation;

			reactphysics3d::Vector3		linVelocity;
			reactphysics3d::Vector3		angVelocity;
			int							stateID;
		};
	}
}

