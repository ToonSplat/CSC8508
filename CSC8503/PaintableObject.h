#pragma once
#include "PaintableZone.h"

namespace NCL {
	namespace CSC8503 {
		class PaintableObject {
			// TODO: Actually link this class to something placable in the world... GameObject?
		public:
			PaintableObject(void);
			PaintableObject(PaintableZone* parent);
			~PaintableObject(void) = default;

			void Hit(Team* hitBy);

		private:
			Team* owner;
			PaintableZone* parent;
			const int OBJECT_SCORE = 1;
		};
	}
}