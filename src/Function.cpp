#include <ShitGraph/Function.hpp>

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
		// TODO: 연속성 확인

		std::vector<Line> result;
		Line* currentLine = nullptr;
		const int sampleCount = static_cast<int>(graph->Independent(context.ViewportPhysical.RightBottom));

		Vector prevDeps;
		bool prevDrawed = false;

		for (int indepP = 0; indepP < sampleCount; ++indepP) {
			const Scalar indep = context.LogicalIndependent(graph, indepP);
			const Vector deps = Solve(graph, indep);
			if (!currentLine && !deps.empty()) {
				currentLine = &result.emplace_back();
			}

			bool drawed = false;
			for (std::size_t i = 0; i < deps.size(); ++i) {
				const Scalar dep = deps[i];
				const bool shouldDraw = ShouldDraw(context, graph, dep);
				if (shouldDraw || prevDrawed) {
					const Scalar depP = context.PhysicalDependent(graph, dep);
					currentLine->push_back(graph->XY(indepP, depP));
				}
				drawed |= shouldDraw;
			}

			if (!prevDrawed && drawed && indepP != 0) {
				for (std::size_t i = 0; i < prevDeps.size(); ++i) {
					const Scalar prevDep = prevDeps[i];
					const Scalar prevDepP = context.PhysicalDependent(graph, prevDep);
					currentLine->insert(currentLine->end() - (currentLine->empty() ? 0 : 1), graph->XY(indepP - 1, prevDepP));
				}
			}

			prevDeps = std::move(deps);
			prevDrawed = drawed;
		}

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