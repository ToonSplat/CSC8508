#pragma once
#include "Vector4.h"
#include "Vector3.h"

using namespace NCL::Maths;

namespace NCL {
	namespace CSC8503 {
		class Light {
		public:
			Light() { }
			Light(Vector4 colour, float radius, Vector3 position) : m_lightColour(colour), m_lightRadius(radius), m_lightPosition(position) { }
			~Light(void) = default;

			Vector4 GetLightColour() const { return m_lightColour; }
			float GetLightRadius() const { return m_lightRadius; }
			Vector3 GetLightPosition() const { return m_lightPosition; }
		protected:
			Vector4 m_lightColour;
			float m_lightRadius;
			Vector3 m_lightPosition;
		};
	}
}


