#include <ShitGraph/CoreType.hpp>

#include <random>

namespace ShitGraph {
	Color RandomColor() noexcept {
		static std::random_device randomDevice;
		static std::mt19937 random(randomDevice());
		static std::uniform_int_distribution dist(0, 255);
		return { static_cast<std::uint8_t>(dist(random)),
			static_cast<std::uint8_t>(dist(random)),
			static_cast<std::uint8_t>(dist(random)) };
	}
}