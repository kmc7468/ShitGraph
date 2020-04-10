#pragma once

#include <vector>

using Scalar = double;
using Vector = std::vector<Scalar>;

struct Point final {
	Scalar X = 0, Y = 0;
};

struct Rect final {
	Point LeftTop, RightBottom;
};