#pragma once

struct Coordinates
{
	Vector2 origin;
	Vector2 size;

	Coordinates(const Coordinates& coord)
	{
		origin = coord.origin;
		size   = coord.size;
	}

	Coordinates(Vector2 org, Vector2 siz)
	{
		origin = org;
		size   = siz;
	}

	Coordinates()
	{
		origin = Vector2(0.0f, 0.0f);
		size   = Vector2(0.0f, 0.0f);
	}

	void ScaleCoordinatesBy(float scale)
	{
		origin.x -= scale;
		origin.y -= scale;
		size.x	 += 2 * scale;
		size.y	 += 2 * scale;
	}
};