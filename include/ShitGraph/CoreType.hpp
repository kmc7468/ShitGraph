#pragma once

#include <vector>

namespace ShitGraph {
	using Scalar = double;
	using Vector = std::vector<Scalar>;

	struct Point final {
		Scalar X = 0, Y = 0;
	};

	struct Rectangle final {
		Point LeftTop, RightBottom;
	};
}