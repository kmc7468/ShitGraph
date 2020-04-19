#include <ShitGraph/Sampler.hpp>

#include <ShitGraph/Graph.hpp>

#include <algorithm>
#include <iterator>

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
		if (graph->IndependentVariable == IndependentVariable::X) return context.Viewport.RightBottom.Y <= dep && dep <= context.Viewport.LeftTop.Y;
		else return context.Viewport.LeftTop.X <= dep && dep <= context.Viewport.RightBottom.X;
	}
	void Sampler::SeparateLines(const SamplingContext& context, const Graph* graph, std::vector<Line>& lines) const {
		std::vector<Line> newLines;

		for (std::size_t i = 0; i < lines.size(); ++i) {
			Line& line = lines[i];
			if (line.size() < 2) continue;

			std::size_t begin = 0;
			for (std::size_t j = 0; j < line.size(); ++j) {
				if (j) {
					const Point from = context.Logical(line[j - 1]);
					const Point to = context.Logical(line[j]);
					if (!graph->IsContinuous(from, to)) {
						Line& newLine = newLines.emplace_back();
						std::copy(line.begin() + begin, line.begin() + j, std::back_inserter(newLine));
						begin = j;
					}
				}
			}

			line.erase(line.begin(), line.begin() + begin);
		}

		lines.insert(lines.end(), newLines.begin(), newLines.end());
	}
}