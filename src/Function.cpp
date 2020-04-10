#include <ShitGraph/Function.hpp>

namespace ShitGraph {
	FunctionParameter::~FunctionParameter() {}

	bool ContinuousFunction(const Point&, const Point&) {
		return true;
	}

	FunctionGraph::FunctionGraph(const FunctionClass& functionClass) noexcept
		: FunctionGraph(functionClass.Parameter, functionClass.CheckContinuity) {}
	FunctionGraph::FunctionGraph(const FunctionParameter* parameter) noexcept
		: FunctionGraph(parameter, ContinuousFunction) {}
	FunctionGraph::FunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity) noexcept
		: m_Parameter(parameter), m_CheckContinuity(checkContinuity) {}
	FunctionGraph::~FunctionGraph() {
		delete m_Parameter;
	}

	bool FunctionGraph::IsContinuous(Point from, Point to) const {
		return m_CheckContinuity(from, to);
	}

	const FunctionParameter* FunctionGraph::GetParameter() const noexcept {
		return m_Parameter;
	}

	ExplicitFunctionGraph::ExplicitFunctionGraph(ExplicitFunctionClass graphClass) noexcept
		: FunctionGraph(graphClass), m_Function(graphClass.Function) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(const FunctionParameter* parameter, ExplicitFunction function) noexcept
		: FunctionGraph(parameter), m_Function(function) {}
	ExplicitFunctionGraph::ExplicitFunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ExplicitFunction function) noexcept
		: FunctionGraph(parameter, checkContinuity), m_Function(function) {}

	void ExplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		Point point{ x };
		if (m_Function(GetParameter(), point)) {
			y.push_back(point.Y);
		}
	}

	ImplicitFunctionGraph::ImplicitFunctionGraph(ImplicitFunctionClass graphClass) noexcept
		: FunctionGraph(graphClass), m_Function(graphClass.Function) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(const FunctionParameter* parameter, ImplicitFunction function) noexcept
		: FunctionGraph(parameter), m_Function(function) {}
	ImplicitFunctionGraph::ImplicitFunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ImplicitFunction function) noexcept
		: FunctionGraph(parameter, checkContinuity), m_Function(function) {}

	void ImplicitFunctionGraph::Solve(Scalar x, Vector& y) const {
		m_Function(GetParameter(), x, y);
	}
}