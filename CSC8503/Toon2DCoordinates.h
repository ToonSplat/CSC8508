#pragma once
using namespace NCL::Maths;
struct Coordinates
{
	Maths::Vector2 origin;
	Maths::Vector2 size;

	Coordinates(const Coordinates& coord)
	{
		origin = coord.origin;
		size   = coord.size;
	}

	Coordinates(Maths::Vector2 org, Maths::Vector2 siz)
	{
		origin = org;
		size   = siz;
	}

	Coordinates()
	{
		origin = Maths::Vector2(0.0f, 0.0f);
		size   = Maths::Vector2(0.0f, 0.0f);
	}

	void ScaleCoordinatesBy(float scale)
	{
		origin.x -= scale;
		origin.y -= scale;
		size.x	 += 2 * scale;
		size.y	 += 2 * scale;
	}

	bool operator==(Coordinates& otherCoordinate)
	{
		return (origin.x == otherCoordinate.origin.x && origin.y == otherCoordinate.origin.y && size.x == otherCoordinate.size.x && size.y == otherCoordinate.size.y);
	}
};