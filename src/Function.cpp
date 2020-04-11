#include <ShitGraph/Function.hpp>

namespace ShitGraph {
	FunctionParameter::~FunctionParameter() {}
}

namespace ShitGraph {
	bool ContinuousFunction(const Point&, const Point&) {
		return true;
	}
}

namespace ShitGraph {
	FunctionGraph::FunctionGraph(const FunctionGraphClass& graphClass) noexcept
		: Graph(graphClass), m_Parameter(graphClass.Parameter), m_CheckContinuity(graphClass.CheckContinuity) {}
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
	ExplicitFunctionGraph::ExplicitFunctionGraph(const ExplicitFunctionClass& graphClass) noexcept
		: FunctionGraph(graphClass), m_Function(graphClass.Function) {}

	void ExplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		Point point{ x };
		if (m_Function(GetParameter(), point)) {
			y.push_back(point.Y);
		}
	}
}

namespace ShitGraph {
	MultivaluedExplicitFunctionGraph::MultivaluedExplicitFunctionGraph(const MultivaluedExplicitFunctionClass& graphClass) noexcept
		: FunctionGraph(graphClass), m_Function(graphClass.Function) {}

	void MultivaluedExplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		m_Function(GetParameter(), x, y);
	}
}