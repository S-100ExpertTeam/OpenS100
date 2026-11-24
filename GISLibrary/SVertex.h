#pragma once

// A vertex of a curve or of the rings of a surface: a position in the coordinates the datasets
// are kept in, and nothing more. A chart has more than a million of these, so a vertex is not a
// geometry object (no identifier, no bounding box, no virtual table): sixteen bytes each, made
// and freed as one block.
struct SVertex
{
	double x = 0;
	double y = 0;

	void SetPoint(double _x, double _y)
	{
		x = _x;
		y = _y;
	}

	[[nodiscard]] double GetX() const { return x; }
	[[nodiscard]] double GetY() const { return y; }

	[[nodiscard]] bool operator==(const SVertex& other) const { return x == other.x && y == other.y; }
};
