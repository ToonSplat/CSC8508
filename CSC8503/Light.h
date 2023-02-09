#pragma once
#include "OGLRenderer.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class Light{
		public:
			Light() {};// Default constructor , we’ll be needing this later!
			Light(const Vector3& position, const Vector4& colour, float radius) {
				this->position = position;
				this->colour = colour;
				this->radius = radius;

			}

			~Light(void) {};

			Vector3 GetPosition() const { return position; }
			void SetPosition(const Vector3& val) { position = val; }

			float GetRadius() const { return radius; }
			void SetRadius(float val) { radius = val; }

			Vector4 GetColour() const { return colour; }
			void SetColour(const Vector4& val) { colour = val; }
		protected:
			Vector3 position;
			float radius;
			Vector4 colour;

		};
	}
}
