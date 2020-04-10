#include <ShitGraph/Graphic.hpp>

#include <algorithm>
#include <cassert>

namespace ShitGraph {
	SolidBrush::SolidBrush(const Color& color) noexcept
		: m_Color(color) {}

	Color SolidBrush::GetColor() const noexcept {
		return m_Color;
	}
}

namespace ShitGraph {
	Pen::Pen(const Color& color) noexcept
		: Pen(color, 1) {}
	Pen::Pen(const Color& color, Scalar width) noexcept
		: m_Color(color), m_Width(width) {}

	Color Pen::GetColor() const noexcept {
		return m_Color;
	}
	Scalar Pen::GetWidth() const noexcept {
		return m_Width;
	}
}

namespace ShitGraph {
	GraphicDevice::GraphicDevice(const ShitGraph::Rectangle& rectangle) noexcept
		: m_Rectangle(rectangle) {}
	GraphicDevice::~GraphicDevice() {
		for (GraphicObject* object : m_Objects) {
			delete object;
		}
	}

	Rectangle GraphicDevice::GetRectangle() const noexcept {
		return m_Rectangle;
	}
	int GraphicDevice::GetWidth() const noexcept {
		return static_cast<int>(m_Rectangle.RightBottom.X);
	}
	int GraphicDevice::GetHeight() const noexcept {
		return static_cast<int>(m_Rectangle.RightBottom.Y);
	}

	ShitGraph::SolidBrush* GraphicDevice::SolidBrush(const Color& color) {
		const auto result = CreateSolidBrush(color);
		return m_Objects.push_back(result), result;
	}
	ShitGraph::Pen* GraphicDevice::Pen(const Color& color) {
		const auto result = Pen(color, 1);
		return m_Objects.push_back(result), result;
	}
	ShitGraph::Pen* GraphicDevice::Pen(const Color& color, Scalar width) {
		const auto result = CreatePen(color, width);
		return m_Objects.push_back(result), result;
	}
	void GraphicDevice::Delete(GraphicObject* object) {
		const auto iter = std::find(m_Objects.begin(), m_Objects.end(), object);
		assert(iter != m_Objects.end());
		m_Objects.erase(iter);

		delete object;
	}
}