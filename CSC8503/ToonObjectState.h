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

			int		position[3];
			short	orientation[4];

			int		linVelocity[3];
			int		angVelocity[3];
			int							stateID;
		};
	}
}

