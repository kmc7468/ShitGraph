#pragma once

#include <ShitGraph/CoreType.hpp>

#include <vector>

namespace ShitGraph {
	struct SamplingContext final {
		Rectangle Viewport;
		Scalar PointsPerPixel;
	};
}

namespace ShitGraph {
	class Graph;

	class Sampler {
	public:
		Sampler() noexcept = default;
		Sampler(const Sampler&) = delete;
		virtual ~Sampler() = default;

	public:
		Sampler& operator=(const Sampler&) = delete;

	public:
		virtual std::vector<Line> Sample(const SamplingContext& context, const Graph* graph) const = 0;
	};
}