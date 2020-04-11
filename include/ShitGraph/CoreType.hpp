#pragma once

#include <cstdint>
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

namespace ShitGraph {
	struct Color final {
		std::uint8_t R = 0;
		std::uint8_t G = 0;
		std::uint8_t B = 0;
	};

	Color RandomColor() noexcept;
}