#include <Function.hpp>

FunctionParameter::~FunctionParameter() {}

bool ContinuousFunction(const Point&, const Point&) {
	return true;
}

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