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

	struct FunctionClass {
		const FunctionParameter* Parameter = nullptr;
		CheckContinuityFunction CheckContinuity = nullptr;
	};

	class FunctionGraph : public Graph {
	private:
		const FunctionParameter* m_Parameter = nullptr;
		CheckContinuityFunction m_CheckContinuity = nullptr;

	public:
		explicit FunctionGraph(const FunctionClass& functionClass) noexcept;
		explicit FunctionGraph(const FunctionParameter* parameter) noexcept;
		FunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity) noexcept;
		FunctionGraph(const FunctionGraph&) = delete;
		virtual ~FunctionGraph() override;

	public:
		FunctionGraph& operator=(const FunctionGraph&) = delete;

	public:
		virtual bool IsContinuous(Point from, Point to) const override;

	protected:
		const FunctionParameter* GetParameter() const noexcept;
	};

	using ExplicitFunction = bool(*)(const FunctionParameter* parameter, Point& point);

	struct ExplicitFunctionClass final : FunctionClass {
		ExplicitFunction Function = nullptr;
	};

	class ExplicitFunctionGraph final : public FunctionGraph {
	private:
		ExplicitFunction m_Function = nullptr;

	public:
		explicit ExplicitFunctionGraph(ExplicitFunctionClass graphClass) noexcept;
		ExplicitFunctionGraph(const FunctionParameter* parameter, ExplicitFunction function) noexcept;
		ExplicitFunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ExplicitFunction function) noexcept;
		ExplicitFunctionGraph(const ExplicitFunctionGraph&) = delete;
		virtual ~ExplicitFunctionGraph() override = default;

	public:
		ExplicitFunctionGraph& operator=(const ExplicitFunctionGraph&) = delete;

	protected:
		virtual void Solve(Scalar x, Vector& y) const override;
	};

	using ImplicitFunction = void(*)(const FunctionParameter* parameter, Scalar x, Vector& y);

	struct ImplicitFunctionClass final : FunctionClass {
		ImplicitFunction Function = nullptr;
	};

	class ImplicitFunctionGraph final : public FunctionGraph {
	private:
		ImplicitFunction m_Function = nullptr;

	public:
		explicit ImplicitFunctionGraph(ImplicitFunctionClass graphClass) noexcept;
		ImplicitFunctionGraph(const FunctionParameter* parameter, ImplicitFunction function) noexcept;
		ImplicitFunctionGraph(const FunctionParameter* parameter, CheckContinuityFunction checkContinuity, ImplicitFunction function) noexcept;
		ImplicitFunctionGraph(const ImplicitFunctionGraph&) = delete;
		virtual ~ImplicitFunctionGraph() override = default;

	public:
		ImplicitFunctionGraph& operator=(const ImplicitFunctionGraph&) = delete;

	protected:
		virtual void Solve(Scalar x, Vector& y) const override;
	};
}