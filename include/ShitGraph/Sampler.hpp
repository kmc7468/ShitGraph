#pragma once

#include <ShitGraph/CoreType.hpp>

#include <vector>

namespace ShitGraph {
	class Graph;

	struct SamplingContext final {
		const Rectangle& Viewport;
		const Rectangle& ViewportPhysical;
		const Point& Center;
		Scalar Scale;

		Point Logical(const Point& point) const noexcept;
		Point Physical(const Point& point) const noexcept;

		Scalar LogicalIndependent(const Graph* graph, Scalar independent) const noexcept;
		Scalar PhysicalDependent(const Graph* graph, Scalar dependent) const noexcept;
	};
}

namespace ShitGraph {
	class Sampler {
	public:
		Sampler() noexcept = default;
		Sampler(const Sampler&) = delete;
		virtual ~Sampler() = default;

	public:
		Sampler& operator=(const Sampler&) = delete;

	public:
		virtual std::vector<Line> Sample(const SamplingContext& context, const Graph* graph) const = 0;

	protected:
		bool ShouldDraw(const SamplingContext& context, const Graph* graph, Scalar dep) const noexcept;
	};
}