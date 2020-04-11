#pragma once

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graphic.hpp>

#include <vector>

namespace ShitGraph {
	enum class IndependentVariable {
		X,
		Y,
	};
}

namespace ShitGraph {
	class Graph {
	private:
		IndependentVariable m_IndependentVariable = IndependentVariable::X;

	public:
		Graph() noexcept = default;
		explicit Graph(IndependentVariable independentVariable) noexcept;
		Graph(const Graph&) = delete;
		virtual ~Graph() = default;

	public:
		Graph& operator=(const Graph&) = delete;

	public:
		Vector Solve(Scalar independent) const;
		bool IsContinuous(Point from, Point to) const;
		IndependentVariable GetIndependentVariable() const noexcept;

	protected:
		virtual void Solve(Scalar x, Vector& y) const = 0;
		virtual bool CheckContinuity(const Point& from, const Point& to) const = 0;
	};
}

namespace ShitGraph {
	class Graphs final {
	private:
		std::vector<Graph*> m_Graphs;
		Point m_Center;
		Scalar m_Scale = 0.25;

	public:
		Graphs() noexcept = default;
		Graphs(const Graphs&) = delete;
		~Graphs() = default;

	public:
		Graphs& operator=(const Graphs&) = delete;

	public:
		Point GetCenter() const noexcept;
		void SetCenter(const Point& newCenter) noexcept;
		Scalar GetScale() const noexcept;
		void SetScale(Scalar newScale) noexcept;

		void AddGraph(Graph* graph);
		void RemoveGraph(Graph* graph);
		void DeleteGraph(Graph* graph);

		void Render(GraphicDevice& device) const;

	private:
		std::vector<std::vector<Point>> GetPoints(const GraphicDevice& device, const Rectangle& rect, const Rectangle& rectP, const Graph* graph) const;
		bool ShouldDraw(const Rectangle& rect, const Graph* graph, Scalar dep) const noexcept;

	private:
		Point Logical(const GraphicDevice& device, const Point& point) const noexcept;
		Rectangle Logical(const GraphicDevice& device, const Rectangle& rectangle) const noexcept;
		Point Physical(const GraphicDevice& device, const Point& point) const noexcept;

		Scalar Independent(const Graph* graph, const Point& point) const noexcept;
		Scalar Dependent(const Graph* graph, const Point& point) const noexcept;
		Point XY(const Graph* graph, const Point& point) const noexcept;
		Scalar LogicalIndependent(const GraphicDevice& device, const Graph* graph, Scalar independent) const noexcept;
		Scalar PhysicalDependent(const GraphicDevice& device, const Graph* graph, Scalar dependent) const noexcept;
	};
}