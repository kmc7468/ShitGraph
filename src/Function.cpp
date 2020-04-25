#include <ShitGraph/Function.hpp>

#include <cmath>

namespace ShitGraph {
	FunctionParameter::~FunctionParameter() {}

	bool ContinuousFunction(const Point&, const Point&) {
		return true;
	}
}

namespace ShitGraph {
	FunctionGraph::FunctionGraph(const Sampler* sampler, const FunctionGraphClass& graphClass) noexcept
		: Graph(sampler, graphClass), m_Parameter(graphClass.Parameter), m_CheckContinuity(graphClass.CheckContinuity) {}
	FunctionGraph::~FunctionGraph() {
		delete m_Parameter;
	}

	bool FunctionGraph::CheckContinuity(const Point& from, const Point& to) const {
		return m_CheckContinuity(from, to);
	}
	const FunctionParameter* FunctionGraph::GetParameter() const noexcept {
		return m_Parameter;
	}
}

namespace ShitGraph {
	template<typename T>
	std::vector<Line> ExplicitFunctionSampler<T>::Sample(const SamplingContext& context, const Graph* graph) const {
		std::vector<Line> result;
		const int sampleCount = static_cast<int>(graph->Independent(context.ViewportPhysical.RightBottom));

		Vector prevDeps;
		bool prevDrawed = false;

		for (int indepP = 0; indepP < sampleCount; ++indepP) {
			const Scalar indep = context.LogicalIndependent(graph, indepP);
			const Vector deps = Solve(graph, indep);
			if (result.empty() && !deps.empty()) {
				result.resize(deps.size());
			}

			bool drawed = false;
			for (std::size_t i = 0; i < deps.size(); ++i) {
				const Scalar dep = deps[i];
				const bool shouldDraw = ShouldDraw(context, graph, dep);
				if (shouldDraw || prevDrawed) {
					const Scalar depP = context.PhysicalDependent(graph, dep);
					result[i].push_back(graph->XY(indepP, depP));
				}
				drawed |= shouldDraw;
			}

			if (!prevDrawed && drawed && indepP != 0) {
				for (std::size_t i = 0; i < prevDeps.size(); ++i) {
					const Scalar prevDep = prevDeps[i];
					const Scalar prevDepP = context.PhysicalDependent(graph, prevDep);
					result[i].insert(result[i].end() - (result[i].empty() ? 0 : 1), graph->XY(indepP - 1, prevDepP));
				}
			}

			prevDeps = std::move(deps);
			prevDrawed = drawed;
		}

		SeparateLines(context, graph, result);
		return result;
	}

	template<typename T>
	Vector ExplicitFunctionSampler<T>::Solve(const Graph* graph, Scalar indep) const {
		Vector deps;
		static_cast<const T*>(graph)->Solve(indep, deps);
		return deps;
	}

	ExplicitFunctionGraph::ExplicitFunctionGraph(const ExplicitFunctionClass& graphClass) noexcept
		: FunctionGraph(new ExplicitFunctionSampler<ExplicitFunctionGraph>, graphClass), m_Function(graphClass.Function) {}

	void ExplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		Point point{ x };
		if (m_Function(GetParameter(), point)) {
			y.push_back(point.Y);
		}
	}

	MultivaluedExplicitFunctionGraph::MultivaluedExplicitFunctionGraph(const MultivaluedExplicitFunctionClass& graphClass) noexcept
		: FunctionGraph(new ExplicitFunctionSampler<MultivaluedExplicitFunctionGraph>, graphClass), m_Function(graphClass.Function) {}

	void MultivaluedExplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		m_Function(GetParameter(), x, y);
	}
}

namespace ShitGraph {
	std::vector<Line> ImplicitFunctionSampler::Sample(const SamplingContext& context, const Graph* graph) const {
		const int width = static_cast<int>(context.ViewportPhysical.RightBottom.X) + 1,
			height = static_cast<int>(context.ViewportPhysical.RightBottom.Y) + 1;
		std::vector<bool> signs(width * height);

		for (int xP = 0; xP < width; ++xP) {
			for (int yP = 0; yP < height; ++yP) {
				static constexpr Scalar half = 0.5;
				const Point point = context.Logical({ xP - half, yP - half });
				signs[xP * height + yP] = std::signbit(Solve(graph, point.X, point.Y));
			}
		}

		std::vector<Line> result;
		std::vector<bool> processed((width - 1) * (height - 1));
		Line* currentLine = nullptr;

		for (int xP = 0; xP < width - 1; ++xP) {
			for (int yP = 0; yP < height - 1; ++yP) {
				if (processed[xP * (height - 1) + yP]) continue;
				processed[xP * (height - 1) + yP] = true;

				static constexpr bool isNear[4][4] = {
					{ false, true, true, false },
					{ true, false, false, true },
					{ true, false, false, true },
					{ false, true, true, false },
				};
				const bool vertex[4] = {
					signs[xP * height + yP], signs[(xP + 1) * height + yP],
					signs[xP * height + yP + 1], signs[(xP + 1) * height + yP + 1],
				};
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						if (!isNear[i][j] || vertex[i] == vertex[j]) continue;
						
						//if (!currentLine) {
							currentLine = &result.emplace_back();
						//}
						currentLine->push_back({ static_cast<Scalar>(xP), static_cast<Scalar>(yP) });
						goto br;
					}
				}
			br:;
			}
		}

		SeparateLines(context, graph, result);
		return result;
	}

	Scalar ImplicitFunctionSampler::Solve(const Graph* graph, Scalar x, Scalar y) const {
		return static_cast<const ImplicitFunctionGraph*>(graph)->Solve({ x, y });
	}

	ImplicitFunctionGraph::ImplicitFunctionGraph(const ImplicitFunctionClass& graphClass) noexcept
		: FunctionGraph(new ImplicitFunctionSampler, graphClass), m_Function(graphClass.Function) {}

	Scalar ImplicitFunctionGraph::Solve(const Point& point) const {
		return m_Function(GetParameter(), point);
	}
}