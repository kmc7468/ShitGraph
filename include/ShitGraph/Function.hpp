#pragma once

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graph.hpp>

namespace ShitGraph {
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

	struct FunctionGraphClass : GraphClass {
		const FunctionParameter* Parameter = nullptr;
		CheckContinuityFunction CheckContinuity = nullptr;
	};

	class FunctionGraph : public Graph {
	private:
		const FunctionParameter* m_Parameter = nullptr;
		CheckContinuityFunction m_CheckContinuity = nullptr;

	public:
		explicit FunctionGraph(const FunctionGraphClass& graphClass) noexcept;
		FunctionGraph(const FunctionGraph&) = delete;
		virtual ~FunctionGraph() override;

	public:
		FunctionGraph& operator=(const FunctionGraph&) = delete;

	protected:
		virtual bool CheckContinuity(const Point& from, const Point& to) const override;
		const FunctionParameter* GetParameter() const noexcept;
	};

	using ExplicitFunction = bool(*)(const FunctionParameter* parameter, Point& point);

	struct ExplicitFunctionClass final : FunctionGraphClass {
		ExplicitFunction Function = nullptr;
	};

	class ExplicitFunctionGraph final : public FunctionGraph {
	private:
		ExplicitFunction m_Function = nullptr;

	public:
		explicit ExplicitFunctionGraph(const ExplicitFunctionClass& graphClass) noexcept;
		ExplicitFunctionGraph(const ExplicitFunctionGraph&) = delete;
		virtual ~ExplicitFunctionGraph() override = default;

	public:
		ExplicitFunctionGraph& operator=(const ExplicitFunctionGraph&) = delete;

	protected:
		virtual void Solve(Scalar x, Vector& y) const override;
	};

	using MultivaluedImplicitFunction = void(*)(const FunctionParameter* parameter, Scalar x, Vector& y);

	struct MultivaluedImplicitFunctionClass final : FunctionGraphClass {
		MultivaluedImplicitFunction Function = nullptr;
	};

	class MultivaluedExplicitFunctionGraph final : public FunctionGraph {
	private:
		MultivaluedImplicitFunction m_Function = nullptr;

	public:
		explicit MultivaluedExplicitFunctionGraph(const MultivaluedImplicitFunctionClass& graphClass) noexcept;
		MultivaluedExplicitFunctionGraph(const MultivaluedExplicitFunctionGraph&) = delete;
		virtual ~MultivaluedExplicitFunctionGraph() override = default;

	public:
		MultivaluedExplicitFunctionGraph& operator=(const MultivaluedExplicitFunctionGraph&) = delete;

	protected:
		virtual void Solve(Scalar x, Vector& y) const override;
	};
}