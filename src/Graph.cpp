#include <ShitGraph/Graph.hpp>

#include <algorithm>
#include <cassert>
#include <memory>

namespace ShitGraph {
	Graph::Graph(const GraphClass& graphClass) noexcept
		: m_IndependentVariable(graphClass.IndependentVariable), m_Color(graphClass.Color) {}

	Vector Graph::Solve(Scalar independent) const {
		Vector dependent;
		Solve(independent, dependent);
		return dependent;
	}
	bool Graph::IsContinuous(Point from, Point to) const {
		if (m_IndependentVariable == IndependentVariable::Y) {
			std::swap(from.X, from.Y);
			std::swap(to.X, to.Y);
		}
		return CheckContinuity(from, to);
	}
	IndependentVariable Graph::GetIndependentVariable() const noexcept {
		return m_IndependentVariable;
	}
	Graph* Graph::SetIndependentVariable(IndependentVariable newIndependentVariable) noexcept {
		m_IndependentVariable = newIndependentVariable;
		return this;
	}

	Color Graph::GetColor() const noexcept {
		return m_Color;
	}
	Graph* Graph::SetColor(Color newColor) noexcept {
		m_Color = newColor;
		return this;
	}

	Graph* MakeForY(Graph* graph) noexcept {
		assert(graph != nullptr);
		return graph->SetIndependentVariable(IndependentVariable::Y), graph;
	}
	Graph* MakeForX(Graph* graph) noexcept {
		assert(graph != nullptr);
		return graph->SetIndependentVariable(IndependentVariable::X), graph;
	}
}

namespace ShitGraph {
	Graphs::Graphs(Scalar scale) noexcept
		: m_Scale(scale) {}
	Point Graphs::GetCenter() const noexcept {
		return m_Center;
	}
	void Graphs::SetCenter(const Point& newCenter) noexcept {
		m_Center = newCenter;
	}
	Scalar Graphs::GetScale() const noexcept {
		return m_Scale;
	}
	void Graphs::SetScale(Scalar newScale) noexcept {
		m_Scale = newScale;
	}

	void Graphs::AddGraph(Graph* graph) {
		m_Graphs.push_back(graph);
	}
	void Graphs::RemoveGraph(Graph* graph) {
		const auto iter = std::find(m_Graphs.begin(), m_Graphs.end(), graph);
		assert(iter != m_Graphs.end());
		m_Graphs.erase(iter);
	}
	void Graphs::DeleteGraph(Graph* graph) {
		RemoveGraph(graph);
		delete graph;
	}

	void Graphs::Render(GraphicDevice& device) const {
		const Rectangle rectP = device.GetRectangle();
		const Rectangle rect = Logical(device, rectP);

		for (const Graph* const graph : m_Graphs) {
			const ManagedGraphicObject<Pen> graphPen(device, device.Pen(graph->GetColor(), 3));
			const std::vector<std::vector<Point>> points = GetPoints(device, rect, rectP, graph);

			for (const auto& area : points) {
				for (std::size_t begin = 0, i = 0; i < area.size(); ++i) {
					if (i == area.size() - 1) {
						device.DrawLines(graphPen, area.data() + begin, i - begin + 1);
						break;
					} else if (i != 0) {
						const Point from = Logical(device, area[i - 1]);
						const Point to = Logical(device, area[i]);
						if (!graph->IsContinuous(from, to)) {
							device.DrawLines(graphPen, area.data() + begin, i - begin);
							begin = i;
						}
					}
				}
			}
		}
	}

	std::vector<std::vector<Point>> Graphs::GetPoints(const GraphicDevice& device, const Rectangle& rect, const Rectangle& rectP, const Graph* graph) const {
		std::vector<std::vector<Point>> points;
		const int pointCount = static_cast<int>(Independent(graph, rectP.RightBottom));

		Vector prevDeps;
		bool prevDrawed = false;

		for (int indepP = 0; indepP < pointCount; ++indepP) {
			const Scalar indep = LogicalIndependent(device, graph, indepP);
			Vector deps = graph->Solve(indep);
			if (points.empty() && !deps.empty()) {
				points.resize(deps.size());
			}

			bool drawed = false;
			for (std::size_t i = 0; i < deps.size(); ++i) {
				const Scalar dep = deps[i];
				const bool shouldDraw = ShouldDraw(rect, graph, dep);
				if (shouldDraw || prevDrawed) {
					const Scalar depP = PhysicalDependent(device, graph, dep);
					points[i].push_back(XY(graph, { static_cast<Scalar>(indepP), depP }));
				}
				drawed |= shouldDraw;
			}

			if (!prevDrawed && drawed && indepP != 0) {
				for (std::size_t i = 0; i < prevDeps.size(); ++i) {
					const Scalar prevDep = prevDeps[i];
					const Scalar prevDepP = PhysicalDependent(device, graph, prevDep);
					points[i].insert(points[i].end() - (points[i].empty() ? 0 : 1), XY(graph, { static_cast<Scalar>(indepP - 1), prevDepP }));
				}
			}

			prevDeps = std::move(deps);
			prevDrawed = drawed;
		}

		return points;
	}
	bool Graphs::ShouldDraw(const Rectangle& rect, const Graph* graph, Scalar dep) const noexcept {
		if (graph->GetIndependentVariable() == IndependentVariable::X) return rect.RightBottom.Y <= dep && dep <= rect.LeftTop.Y;
		else return rect.LeftTop.X <= dep && dep <= rect.RightBottom.X;
	}

	Point Graphs::Logical(int width, int height, const Point& point) const noexcept {
		return {
			(point.X - width / 2.0) * m_Scale - m_Center.X,
			(height / 2.0 - point.Y) * m_Scale - m_Center.Y,
		};
	}
	Point Graphs::Physical(int width, int height, const Point& point) const noexcept {
		return {
			width / 2.0 + (point.X + m_Center.X) / m_Scale,
			height / 2.0 - (point.Y + m_Center.Y) / m_Scale
		};
	}

	Point Graphs::Logical(const GraphicDevice& device, const Point& point) const noexcept {
		return Logical(device.GetWidth(), device.GetHeight(), point);
	}
	Rectangle Graphs::Logical(const GraphicDevice& device, const Rectangle& rectangle) const noexcept {
		return {
			Logical(device, rectangle.LeftTop),
			Logical(device, rectangle.RightBottom)
		};
	}
	Point Graphs::Physical(const GraphicDevice& device, const Point& point) const noexcept {
		return Physical(device.GetWidth(), device.GetHeight(), point);
	}

	Scalar Graphs::Independent(const Graph* graph, const Point& point) const noexcept {
		return graph->GetIndependentVariable() == IndependentVariable::X ? point.X : point.Y;
	}
	Scalar Graphs::Dependent(const Graph* graph, const Point& point) const noexcept {
		return graph->GetIndependentVariable() == IndependentVariable::Y ? point.X : point.Y;
	}
	Point Graphs::XY(const Graph* graph, const Point& point) const noexcept {
		return graph->GetIndependentVariable() == IndependentVariable::X ? point : Point{ point.Y, point.X };
	}
	Scalar Graphs::LogicalIndependent(const GraphicDevice& device, const Graph* graph, Scalar independent) const noexcept {
		return Independent(graph, Logical(device,
			graph->GetIndependentVariable() == IndependentVariable::X ? Point{ independent, 0 } : Point{ 0, independent }));
	}
	Scalar Graphs::PhysicalDependent(const GraphicDevice& device, const Graph* graph, Scalar dependent) const noexcept {
		return Dependent(graph, Physical(device,
			graph->GetIndependentVariable() == IndependentVariable::X ? Point{ 0, dependent } : Point{ dependent, 0 }));
	}
}