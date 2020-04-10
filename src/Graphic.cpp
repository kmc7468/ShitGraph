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
	GraphicDevice::~GraphicDevice() {
		for (GraphicObject* object : m_Objects) {
			delete object;
		}
	}

	int GraphicDevice::GetWidth() const noexcept {
		return static_cast<int>(GetRectangle().RightBottom.X);
	}
	int GraphicDevice::GetHeight() const noexcept {
		return static_cast<int>(GetRectangle().RightBottom.Y);
	}

	ShitGraph::SolidBrush* GraphicDevice::SolidBrush(const Color& color) {
		return CreateSolidBrush(color);
	}
	ShitGraph::Pen* GraphicDevice::Pen(const Color& color) {
		return Pen(color, 1);
	}
	ShitGraph::Pen* GraphicDevice::Pen(const Color& color, Scalar width) {
		return CreatePen(color, width);
	}
	void GraphicDevice::Delete(GraphicObject* object) {
		const auto iter = std::find(m_Objects.begin(), m_Objects.end(), object);
		assert(iter != m_Objects.end());
		m_Objects.erase(iter);

		delete object;
	}
}