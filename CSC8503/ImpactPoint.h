#pragma once
#include "Vector3.h"

using namespace NCL::Maths;

class ImpactPoint {
public:
	ImpactPoint(NCL::Maths::Vector3 location, NCL::Maths::Vector3 colour, float radius) : location(location), colour(colour), radius(radius) {}
	NCL::Maths::Vector3 GetImpactLocation() const { return location; }
	void SetImpactLocation(NCL::Maths::Vector3 localPoint) { location = localPoint; }
	NCL::Maths::Vector3 GetImpactColour() const { return colour; }
	float GetImpactRadius() const { return radius; }
protected:
	NCL::Maths::Vector3 location;
	NCL::Maths::Vector3 colour;
	float radius;
};