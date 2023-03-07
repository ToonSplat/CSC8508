#pragma once

namespace NCL::CSC8503 {
	class ToonObjectState {
	public:
		ToonObjectState() {
			stateID = 0;
		}

		~ToonObjectState() {
		}

		int		position[3];
		short	orientation[4]; // TODO: Can this be char?

		int		linVelocity[3]; // TODO: Can this be short?
		int		angVelocity[3]; // TODO: Can this be short?
		int		stateID; // TODO: Can this be short?
	};
}
