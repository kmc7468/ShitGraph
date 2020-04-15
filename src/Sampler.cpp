#include <ShitGraph/Sampler.hpp>

#include <ShitGraph/Graph.hpp>

namespace ShitGraph {
	Point SamplingContext::Logical(const Point& point) const noexcept {
		const Scalar& width = ViewportPhysical.RightBottom.X;
		const Scalar& height = ViewportPhysical.RightBottom.Y;
		return {
			(point.X - width / 2.0) * Scale - Center.X,
			(height / 2.0 - point.Y) * Scale - Center.Y,
		};
	}
	Point SamplingContext::Physical(const Point& point) const noexcept {
		const Scalar& width = ViewportPhysical.RightBottom.X;
		const Scalar& height = ViewportPhysical.RightBottom.Y;
		return {
			width / 2.0 + (point.X + Center.X) / Scale,
			height / 2.0 - (point.Y + Center.Y) / Scale
		};
	}

	Scalar SamplingContext::LogicalIndependent(const Graph* graph, Scalar independent) const noexcept {
		return graph->Independent(Logical(
			graph->IndependentVariable == IndependentVariable::X ? Point{ independent, 0 } : Point{ 0, independent }));
	}
	Scalar SamplingContext::PhysicalDependent(const Graph* graph, Scalar dependent) const noexcept {
		return graph->Dependent(Physical(
			graph->IndependentVariable == IndependentVariable::X ? Point{ 0, dependent } : Point{ dependent, 0 }));
	}
}

namespace ShitGraph {
	bool Sampler::ShouldDraw(const SamplingContext& context, const Graph* graph, Scalar dep) const noexcept {
		if (graph->IndependentVariable == IndependentVariable::X) return context.ViewportPhysical.RightBottom.Y <= dep && dep <= context.ViewportPhysical.LeftTop.Y;
		else return context.ViewportPhysical.LeftTop.X <= dep && dep <= context.ViewportPhysical.RightBottom.X;
	}
}