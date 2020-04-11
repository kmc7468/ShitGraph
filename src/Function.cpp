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
	FunctionGraph::FunctionGraph(const FunctionClass& functionClass) noexcept
		: FunctionGraph(IndependentVariable::X, functionClass.Parameter, functionClass.CheckContinuity) {}
	FunctionGraph::FunctionGraph(IndependentVariable independentVariable, const FunctionClass& functionClass) noexcept
		: FunctionGraph(independentVariable, functionClass.Parameter, functionClass.CheckContinuity) {}
	FunctionGraph::FunctionGraph(const FunctionParameter* parameter) noexcept
		: FunctionGraph(IndependentVariable::X, parameter, ContinuousFunction) {}
	FunctionGraph::FunctionGraph(IndependentVariable independentVariable, const FunctionParameter* parameter) noexcept
		: FunctionGraph(independentVariable, parameter, ContinuousFunction) {}
	FunctionGraph::FunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity) noexcept
		: FunctionGraph(IndependentVariable::X, parameter, checkContinuity) {}
	FunctionGraph::FunctionGraph(IndependentVariable independentVariable, const FunctionParameter* parameter, CheckContinuityFunction checkContinuity) noexcept
		: Graph(independentVariable), m_Parameter(parameter), m_CheckContinuity(checkContinuity) {}
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
	ExplicitFunctionGraph::ExplicitFunctionGraph(ExplicitFunctionClass graphClass) noexcept
		: ExplicitFunctionGraph(IndependentVariable::X, graphClass) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(IndependentVariable independentVariable, ExplicitFunctionClass graphClass) noexcept
		: FunctionGraph(independentVariable, graphClass), m_Function(graphClass.Function) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(const FunctionParameter* parameter, ExplicitFunction function) noexcept
		: ExplicitFunctionGraph(IndependentVariable::X, parameter, ContinuousFunction, function) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(IndependentVariable independentVariable, const FunctionParameter* parameter, ExplicitFunction function) noexcept
		: ExplicitFunctionGraph(independentVariable, parameter, ContinuousFunction, function) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ExplicitFunction function) noexcept
		: ExplicitFunctionGraph(IndependentVariable::X, parameter, checkContinuity, function) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(IndependentVariable independentVariable, const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ExplicitFunction function) noexcept
		: FunctionGraph(independentVariable, parameter, checkContinuity), m_Function(function) {}

	void ExplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		Point point{ x };
		if (m_Function(GetParameter(), point)) {
			y.push_back(point.Y);
		}
	}
}

namespace ShitGraph {
	ImplicitFunctionGraph::ImplicitFunctionGraph(ImplicitFunctionClass graphClass) noexcept
		: ImplicitFunctionGraph(IndependentVariable::X, graphClass) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(IndependentVariable independentVariable, ImplicitFunctionClass graphClass) noexcept
		: FunctionGraph(independentVariable, graphClass), m_Function(graphClass.Function) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(const FunctionParameter* parameter, ImplicitFunction function) noexcept
		: ImplicitFunctionGraph(IndependentVariable::X, parameter, ContinuousFunction, function) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(IndependentVariable independentVariable, const FunctionParameter* parameter, ImplicitFunction function) noexcept
		: ImplicitFunctionGraph(independentVariable, parameter, ContinuousFunction, function) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ImplicitFunction function) noexcept
		: ImplicitFunctionGraph(IndependentVariable::X, parameter, checkContinuity, function) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(IndependentVariable independentVariable, const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ImplicitFunction function) noexcept
		: FunctionGraph(independentVariable, parameter, checkContinuity), m_Function(function) {}

	void ImplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		m_Function(GetParameter(), x, y);
	}
}