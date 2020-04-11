#include <ShitGraph/Graph.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>

namespace ShitGraph {
	Graph::Graph(IndependentVariable independentVariable) noexcept
		: m_IndependentVariable(independentVariable) {}

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
}

namespace ShitGraph {
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

	void Graphs::Render(GraphicDevice& device) {
		// TODO

		const ManagedGraphicObject<Pen> graphPen(device, device.Pen({ 0, 0, 0 }, 2));

		const Rectangle rectP = device.GetRectangle();
		const Rectangle rect = Logical(device, rectP);

		for (const Graph* const graph : m_Graphs) {
			std::vector<std::vector<Point>> points;

			Scalar prevXP = 0;
			Vector prevYs;
			bool prevDrawed = false;

			for (int xP = 0; xP < static_cast<int>(rectP.RightBottom.X); ++xP) {
				const Scalar x = Logical(device, Point{ static_cast<Scalar>(xP) }).X;
				Vector ys = graph->Solve(x);
				if (points.empty() && !ys.empty()) {
					points.resize(ys.size());
				}

				bool drawed = false;
				for (std::size_t i = 0; i < ys.size(); ++i) {
					const Scalar y = ys[i];
					const bool shouldDraw = rect.RightBottom.Y <= y && y <= rect.LeftTop.Y;
					if (shouldDraw || prevDrawed) {
						const Scalar yP = Physical(device, { 0, y }).Y;
						points[i].push_back({ static_cast<Scalar>(xP), yP });
					}
					drawed |= shouldDraw;
				}

				if (!prevDrawed && drawed && xP != 0) {
					for (std::size_t i = 0; i < prevYs.size(); ++i) {
						const Scalar y = prevYs[i];
						const double yP = Physical(device, { 0, y }).Y;
						points[i].insert(points[i].end() - (points[i].empty() ? 0 : 1), { prevXP, yP });
					}
				}

				prevXP = xP;
				prevYs = std::move(ys);
				prevDrawed = drawed;
			}

			for (std::size_t i = 0; i < points.size(); ++i) {
				for (std::size_t begin = 0, j = 1; j < points[i].size(); ++j) {
					const Point from = Logical(device, points[i][j - 1]);
					const Point to = Logical(device, points[i][j]);

					if (j == points[i].size() - 1) {
						device.DrawLines(graphPen, points[i].data() + begin, j - begin + 1);
					} else if (!graph->IsContinuous(from, to)) {
						device.DrawLines(graphPen, points[i].data() + begin, j - begin);
						begin = j;
					}
				}
			}
		}
	}

	Point Graphs::Logical(const GraphicDevice& device, const Point& point) const noexcept {
		return {
			(point.X - device.GetWidth() / 2.0) * m_Scale - m_Center.X,
			(device.GetHeight() / 2.0 - point.Y) * m_Scale - m_Center.Y,
		};
	}
	Rectangle Graphs::Logical(const GraphicDevice& device, const Rectangle& rectangle) const noexcept {
		return {
			Logical(device, rectangle.LeftTop),
			Logical(device, rectangle.RightBottom)
		};
	}
	Point Graphs::Physical(const GraphicDevice& device, const Point& point) const noexcept {
		return {
			device.GetWidth() / 2.0 + (point.X + m_Center.X) / m_Scale,
			device.GetHeight() / 2.0 - (point.Y + m_Center.Y) / m_Scale
		};
	}
}