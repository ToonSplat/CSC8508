#pragma once
#include "Vector3.h"

using namespace NCL::Maths;

class ImpactPoint {
public:
	ImpactPoint(Vector3 location, Vector3 colour, float radius) : location(location), colour(colour), radius(radius) {}
	Vector3 GetImpactLocation() const { return location; }
	void SetImpactLocation(Vector3 localPoint) { location = localPoint; }
	Vector3 GetImpactColour() const { return colour; }
	float GetImpactRadius() const { return radius; }
protected:
	Vector3 location;
	Vector3 colour;
	float radius;
};