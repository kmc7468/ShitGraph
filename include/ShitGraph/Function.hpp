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

	using ImplicitFunction = void(*)(const FunctionParameter* parameter, Scalar x, Vector& y);

	struct ImplicitFunctionClass final : FunctionGraphClass {
		ImplicitFunction Function = nullptr;
	};

	class ImplicitFunctionGraph final : public FunctionGraph {
	private:
		ImplicitFunction m_Function = nullptr;

	public:
		explicit ImplicitFunctionGraph(const ImplicitFunctionClass& graphClass) noexcept;
		ImplicitFunctionGraph(const ImplicitFunctionGraph&) = delete;
		virtual ~ImplicitFunctionGraph() override = default;

	public:
		ImplicitFunctionGraph& operator=(const ImplicitFunctionGraph&) = delete;

	protected:
		virtual void Solve(Scalar x, Vector& y) const override;
	};
}