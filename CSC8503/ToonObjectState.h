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

			reactphysics3d::Vector3		position;
			reactphysics3d::Quaternion	orientation;
			int			stateID;
		};
	}
}

