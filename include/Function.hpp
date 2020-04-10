#pragma once

#include <CoreType.hpp>
#include <Graph.hpp>

class FunctionParameter {
public:
	FunctionParameter() noexcept = default;
	FunctionParameter(const FunctionParameter&) = delete;
	virtual ~FunctionParameter() = 0;

public:
	FunctionParameter& operator=(const FunctionParameter&) = delete;

public:
	template<typename T>
	const T* Cast() const noexcept {
		return static_cast<const T*>(this);
	}
};

using CheckContinuityFunction = bool(*)(const Point& from, const Point& to);

bool ContinuousFunction(const Point& from, const Point& to);

class FunctionGraph : public Graph {
private:
	const FunctionParameter* m_Parameter = nullptr;
	CheckContinuityFunction m_CheckContinuity = nullptr;

public:
	explicit FunctionGraph(const FunctionParameter* parameter) noexcept;
	FunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity) noexcept;
	FunctionGraph(const FunctionGraph&) = delete;
	virtual ~FunctionGraph() override;

public:
	FunctionGraph& operator=(const FunctionGraph&) = delete;

public:
	virtual bool IsContinuous(Point from, Point to) const override;

protected:
	template<typename T>
	const T* GetParameter() const noexcept {
		return m_Parameter->Cast<T>();
	}
};