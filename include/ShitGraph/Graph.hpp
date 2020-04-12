#pragma once

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graphic.hpp>

#include <type_traits>
#include <utility>
#include <vector>

namespace ShitGraph {
	enum class IndependentVariable {
		X,
		Y,
	};
}

namespace ShitGraph {
	struct GraphClass {
		ShitGraph::IndependentVariable IndependentVariable = IndependentVariable::X;

		ShitGraph::Color Color;
		Scalar Width = 2;
		bool Visible = true;
	};

	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeForY(T&& graphClass) noexcept {
		return graphClass.IndependentVariable = IndependentVariable::Y, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeForX(T&& graphClass) noexcept {
		return graphClass.IndependentVariable = IndependentVariable::X, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> ChangeColor(T&& graphClass, Color color) noexcept {
		return graphClass.Color = color, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> ChangeWidth(T&& graphClass, Scalar width) noexcept {
		return graphClass.Width = width, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeVisible(T&& graphClass) noexcept {
		return graphClass.Visible = true, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeInvisible(T&& graphClass) noexcept {
		return graphClass.Visible = false, std::forward<T>(graphClass);
	}

	class Graph : public GraphClass {
	public:
		explicit Graph(const GraphClass& graphClass) noexcept;
		Graph(const Graph&) = delete;
		virtual ~Graph() = default;

	public:
		Graph& operator=(const Graph&) = delete;

	public:
		Vector Solve(Scalar independent) const;
		bool IsContinuous(Point from, Point to) const;

		Graph* MakeForY() noexcept;
		Graph* MakeForX() noexcept;

		Graph* ChangeColor(ShitGraph::Color newColor) noexcept;
		Graph* ChangeWidth(Scalar newWidth) noexcept;
		bool MakeVisible() noexcept;
		bool MakeInvisible() noexcept;

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
		Scalar m_Scale;

	public:
		Graphs(Scalar scale = 0.262144) noexcept;
		Graphs(const Graphs&) = delete;
		~Graphs() = default;

	public:
		Graphs& operator=(const Graphs&) = delete;

	public:
		Point GetCenter() const noexcept;
		void SetCenter(const Point& newCenter) noexcept;
		Scalar GetScale() const noexcept;
		void SetScale(Scalar newScale) noexcept;

		const Graph* GetGraph(std::size_t index) const noexcept;
		Graph* GetGraph(std::size_t index) noexcept;
		std::size_t GetGraphCount() const noexcept;
		void AddGraph(Graph* graph);
		void RemoveGraph(Graph* graph);
		void DeleteGraph(Graph* graph);

		void Render(GraphicDevice& device) const;

	private:
		std::vector<std::vector<Point>> GetPoints(const GraphicDevice& device, const Rectangle& rect, const Rectangle& rectP, const Graph* graph) const;
		bool ShouldDraw(const Rectangle& rect, const Graph* graph, Scalar dep) const noexcept;

	public:
		Point Logical(int width, int height, const Point& point) const noexcept;
		Point Physical(int width, int height, const Point& point) const noexcept;

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