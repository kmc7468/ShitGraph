#include <ShitGraph/Graph.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>

namespace ShitGraph {
	Graph::Graph(const Sampler* sampler, const GraphClass& graphClass) noexcept
		: GraphClass(graphClass), m_Sampler(sampler) {}
	Graph::~Graph() {
		delete m_Sampler;
	}

	std::vector<Line> Graph::Sample(const SamplingContext& context) const {
		if (!Visible) return {};
		else return m_Sampler->Sample(context, this);
	}
	bool Graph::IsContinuous(Point from, Point to) const {
		if (IndependentVariable == ShitGraph::IndependentVariable::Y) {
			std::swap(from.X, from.Y);
			std::swap(to.X, to.Y);
		}
		return CheckContinuity(from, to);
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
				if (line.size() == 1) {
					device.DrawPoint(graphBrush, line.front(), graph->Width);
				} else if (line.size() > 1) {
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

namespace ShitGraph {
	const Graphs& Renderer::GetGraphs() const noexcept {
		return m_Graphs;
	}
	Graphs& Renderer::GetGraphs() noexcept {
		return m_Graphs;
	}

	void Renderer::Paint(PaintEventArgs e) {
		const ManagedGraphicObject<Brush> blackBrush(e.Device, e.Device.SolidBrush({ 0, 0, 0 }));
		const ManagedGraphicObject<Font> fpsFont(e.Device, e.Device.Font("Arial", 12));

		const auto start = std::chrono::system_clock::now();
		m_Graphs.Render(e.Device);
		const auto end = std::chrono::system_clock::now();

		const std::chrono::duration<double, std::milli> milli = end - start;
		const int fps = static_cast<int>(1000 / milli.count());
		e.Device.DrawString(fpsFont, blackBrush, { 0, 0 }, std::to_string(fps) + " fps");
		if (m_CurrentIndex) {
			e.Device.DrawString(fpsFont, blackBrush, { 0, 20 }, "Graph: " + std::to_string(*m_CurrentIndex + 1) + '/' + std::to_string(m_Graphs.GetGraphCount()));
			e.Device.DrawString(fpsFont, blackBrush, { 0, 40 }, m_OriginalVisible ? "Visible" : "Invisible");
		}
	}
	void Renderer::Destroy(EventArgs e) {
		assert(dynamic_cast<Window*>(&e.Control) != nullptr);
		static_cast<Window&>(e.Control).Exit();
	}

	void Renderer::MouseDown(MouseEventArgs e) {
		m_MouseX = e.X;
		m_MouseY = e.Y;
		m_IsMoving = true;
	}
	void Renderer::MouseUp(MouseEventArgs) {
		m_IsMoving = false;
	}
	void Renderer::MouseMove(MouseEventArgs e) {
		if (m_IsMoving) {
			const Point center = m_Graphs.GetCenter();
			m_Graphs.SetCenter({
				center.X - (m_MouseX - e.X) * m_Graphs.GetScale(),
				center.Y + (m_MouseY - e.Y) * m_Graphs.GetScale()
			});
			e.Control.ReDraw();
		}
		m_MouseX = e.X;
		m_MouseY = e.Y;
	}
	void Renderer::MouseWheel(MouseWheelEventArgs e) {
		const Rectangle clientRect = e.Control.GetClientRect();
		const Point mouse = m_Graphs.Logical(static_cast<int>(clientRect.RightBottom.X), static_cast<int>(clientRect.RightBottom.Y),
			{ static_cast<Scalar>(m_MouseX), static_cast<Scalar>(m_MouseY) });
		const Scalar delta = e.Delta > 0 ? 1 / MAGNIFICATION : MAGNIFICATION;
		m_Graphs.SetScale(m_Graphs.GetScale() * delta);

		if (e.ControlKey) {
			const Point newMouse = m_Graphs.Logical(static_cast<int>(clientRect.RightBottom.X), static_cast<int>(clientRect.RightBottom.Y),
				{ static_cast<Scalar>(m_MouseX), static_cast<Scalar>(m_MouseY) });
			const Point center = m_Graphs.GetCenter();
			m_Graphs.SetCenter({
				center.X + (newMouse.X - mouse.X),
				center.Y + (newMouse.Y - mouse.Y),
			});
		}

		e.Control.ReDraw();
	}

	void Renderer::KeyDown(KeyEventArgs e) {
		switch (e.Key) {
		case 'O':
			m_Graphs.SetCenter({ 0, 0 });
			e.Control.ReDraw();
			break;

		case 'R':
			m_Graphs.SetScale(INITIALLY_SCALE);
			e.Control.ReDraw();
			break;

		case EscKey:
			if (m_CurrentIndex) {
				Unselect(*m_CurrentIndex);
				m_CurrentIndex.reset();
				e.Control.ReDraw();
			}
			break;

		case UpKey: SetVisible(e.Control, true); break;
		case DownKey: SetVisible(e.Control, false); break;

		case LeftKey:
			if (!m_CurrentIndex && m_Graphs.GetGraphCount()) {
				const auto index = m_Graphs.GetGraphCount() - 1;
				Select((m_CurrentIndex = index, index));
				e.Control.ReDraw();
			} else if (m_CurrentIndex && *m_CurrentIndex) {
				Unselect(*m_CurrentIndex);
				Select(*m_CurrentIndex -= 1);
				e.Control.ReDraw();
			}
			break;

		case RightKey:
			if (!m_CurrentIndex && m_Graphs.GetGraphCount()) {
				Select((m_CurrentIndex = 0, 0));
				e.Control.ReDraw();
			} else if (m_CurrentIndex && m_CurrentIndex < m_Graphs.GetGraphCount() - 1) {
				Unselect(*m_CurrentIndex);
				Select(*m_CurrentIndex += 1);
				e.Control.ReDraw();
			}
			break;
		}
	}

	void Renderer::SetVisible(Control& control, bool newVisible) noexcept {
		if (m_CurrentIndex && m_OriginalVisible != newVisible) {
			newVisible = m_OriginalVisible;
			control.ReDraw();
		}
	}
	void Renderer::Select(std::size_t index) {
		const auto graph = m_Graphs.GetGraph(index);
		graph->Width += 2;
		m_OriginalVisible = graph->Visible;
		graph->Visible = true;
	}
	void Renderer::Unselect(std::size_t index) {
		const auto graph = m_Graphs.GetGraph(index);
		graph->Width -= 2;
		graph->Visible = m_OriginalVisible;
	}
}