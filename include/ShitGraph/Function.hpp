#pragma once

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graph.hpp>
#include <ShitGraph/Sampler.hpp>

#include <vector>

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
}

namespace ShitGraph {
	struct FunctionGraphClass : GraphClass {
		const FunctionParameter* Parameter = nullptr;
		CheckContinuityFunction CheckContinuity = nullptr;
	};

	class FunctionGraph : public Graph {
	private:
		const FunctionParameter* m_Parameter = nullptr;
		CheckContinuityFunction m_CheckContinuity = nullptr;

	public:
		FunctionGraph(const Sampler* sampler, const FunctionGraphClass& graphClass) noexcept;
		FunctionGraph(const FunctionGraph&) = delete;
		virtual ~FunctionGraph() override;

	public:
		FunctionGraph& operator=(const FunctionGraph&) = delete;

	protected:
		virtual bool CheckContinuity(const Point& from, const Point& to) const override;
		const FunctionParameter* GetParameter() const noexcept;
	};
}

namespace ShitGraph {
	template<typename T>
	class ExplicitFunctionSampler final : public Sampler {
	public:
		ExplicitFunctionSampler() noexcept = default;
		ExplicitFunctionSampler(const ExplicitFunctionSampler&) = delete;
		virtual ~ExplicitFunctionSampler() override = default;

	public:
		ExplicitFunctionSampler& operator=(const ExplicitFunctionSampler&) = delete;

	public:
		virtual std::vector<Line> Sample(const SamplingContext& context, const Graph* graph) const override;

	private:
		Vector Solve(const Graph* graph, Scalar indep) const;
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

	public:
		void Solve(Scalar x, Vector& y) const;
	};

	using MultivaluedExplicitFunction = void(*)(const FunctionParameter* parameter, Scalar x, Vector& y);

	struct MultivaluedExplicitFunctionClass final : FunctionGraphClass {
		MultivaluedExplicitFunction Function = nullptr;
	};

	class MultivaluedExplicitFunctionGraph final : public FunctionGraph {
	private:
		MultivaluedExplicitFunction m_Function = nullptr;

	public:
		explicit MultivaluedExplicitFunctionGraph(const MultivaluedExplicitFunctionClass& graphClass) noexcept;
		MultivaluedExplicitFunctionGraph(const MultivaluedExplicitFunctionGraph&) = delete;
		virtual ~MultivaluedExplicitFunctionGraph() override = default;

	public:
		MultivaluedExplicitFunctionGraph& operator=(const MultivaluedExplicitFunctionGraph&) = delete;

	public:
		void Solve(Scalar x, Vector& y) const;
	};
}

namespace ShitGraph {
	class ImplicitFunctionSampler final : public Sampler {
	public:
		ImplicitFunctionSampler() noexcept = default;
		ImplicitFunctionSampler(const ImplicitFunctionSampler&) = delete;
		virtual ~ImplicitFunctionSampler() override = default;

	public:
		ImplicitFunctionSampler& operator=(const ImplicitFunctionSampler&) = delete;

	public:
		virtual std::vector<Line> Sample(const SamplingContext& context, const Graph* graph) const override;
	};

	using ImplicitFunction = bool(*)(const Point& point);

	struct ImplicitFunctionClass : FunctionGraphClass {
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

	public:
		bool CheckTrue(const Point& point) const;
	};
}