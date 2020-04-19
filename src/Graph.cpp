#include <ShitGraph/Graph.hpp>

#include <algorithm>
#include <cassert>

namespace ShitGraph {
	Graph::Graph(const Sampler* sampler, const GraphClass& graphClass) noexcept
		: GraphClass(graphClass), m_Sampler(sampler) {}

	std::vector<Line> Graph::Sample(const SamplingContext& context) const {
		if (!Visible) return {};
		else return m_Sampler->Sample(context, this);
	}

	Scalar Graph::Independent(const Point& point) const noexcept {
		return IndependentVariable == IndependentVariable::X ? point.X : point.Y;
	}
	Scalar Graph::Dependent(const Point& point) const noexcept {
		return IndependentVariable == IndependentVariable::Y ? point.X : point.Y;
	}
	Point Graph::XY(Scalar independent, Scalar dependent) const noexcept {
		if (IndependentVariable == ShitGraph::IndependentVariable::X) return { independent, dependent };
		else return { dependent, independent };
	}

	Graph* Graph::MakeForY() noexcept {
		return ShitGraph::MakeForY(*this), this;
	}
	Graph* Graph::MakeForX() noexcept {
		return ShitGraph::MakeForX(*this), this;
	}

	Graph* Graph::ChangeColor(ShitGraph::Color newColor) noexcept {
		return ShitGraph::ChangeColor(*this, newColor), this;
	}
	Graph* Graph::ChangeWidth(Scalar newWidth) noexcept {
		return ShitGraph::ChangeWidth(*this, newWidth), this;
	}
	bool Graph::MakeVisible() noexcept {
		return ShitGraph::MakeVisible(*this), this;
	}
	bool Graph::MakeInvisible() noexcept {
		return ShitGraph::MakeInvisible(*this), this;
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

	const Graph* Graphs::GetGraph(std::size_t index) const noexcept {
		return m_Graphs[index];
	}
	Graph* Graphs::GetGraph(std::size_t index) noexcept {
		return m_Graphs[index];
	}
	std::size_t Graphs::GetGraphCount() const noexcept {
		return m_Graphs.size();
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
		const SamplingContext samplingContext{ rect, rectP, m_Center, m_Scale };

		for (const Graph* const graph : m_Graphs) {
			const ManagedGraphicObject<Pen> graphPen(device, device.Pen(graph->Color, graph->Width));
			const ManagedGraphicObject<Brush> graphBrush(device, device.SolidBrush(graph->Color));

			const std::vector<Line> lines = graph->Sample(samplingContext);
			for (const Line& line : lines) {
				if (line.empty()) continue;
				else if (line.size() == 1) {
					device.DrawPoint(graphBrush, line.front(), graph->Width);
				} else {
					device.DrawLines(graphPen, line.data(), line.size());
				}
			}
		}
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

	Rectangle Graphs::Logical(const GraphicDevice& device, const Rectangle& rectangle) const noexcept {
		return { Logical(device.GetWidth(), device.GetHeight(), rectangle.LeftTop),
			Logical(device.GetWidth(), device.GetHeight(), rectangle.RightBottom) };
	}
}