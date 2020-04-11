#include <ShitGraph/platform/win32/Graphic.hpp>

#ifdef SHITGRAPH_WIN32
#include <vector>

namespace {
	Gdiplus::Color GdiplusObject(const ShitGraph::Color& color) noexcept {
		return Gdiplus::Color(color.R, color.G, color.B);
	}
	Gdiplus::PointF GdiplusObject(const ShitGraph::Point& point) noexcept {
		return Gdiplus::PointF(static_cast<float>(point.X), static_cast<float>(point.Y));
	}
}

namespace ShitGraph {
	Win32SolidBrush::Win32SolidBrush(const Color& color)
		: SolidBrush(color), m_Handle(GdiplusObject(color)) {}

	void* Win32SolidBrush::GetHandle() const noexcept {
		return const_cast<decltype(m_Handle)*>(&m_Handle);
	}
}

namespace ShitGraph {
	Win32Pen::Win32Pen(const Color& color)
		: Win32Pen(color, 1) {}
	Win32Pen::Win32Pen(const Color& color, Scalar width)
		: Pen(color), m_Brush(color), m_Handle(static_cast<Gdiplus::Brush*>(m_Brush.GetHandle()), static_cast<Gdiplus::REAL>(width)) {}

	void* Win32Pen::GetHandle() const noexcept {
		return const_cast<decltype(m_Handle)*>(&m_Handle);
	}
}

namespace ShitGraph {
	Win32GraphicDevice::Win32GraphicDevice(HWND window, HDC dc, const ShitGraph::Rectangle& rectangle)
		: GraphicDevice(rectangle), m_Window(window), m_Graphics(dc) {
		m_Graphics.Clear(Gdiplus::Color::White);
		m_Graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	}

	void Win32GraphicDevice::DrawLines(const ShitGraph::Pen* pen, const Point* points, std::size_t size) {
		std::vector<Gdiplus::PointF> pointsGdi(size);
		for (std::size_t i = 0; i < size; ++i) {
			pointsGdi[i] = GdiplusObject(points[i]);
		}

		m_Graphics.DrawLines(static_cast<Gdiplus::Pen*>(pen->GetHandle()), pointsGdi.data(), static_cast<INT>(size));
	}

	ShitGraph::SolidBrush* Win32GraphicDevice::CreateSolidBrush(const Color& color) {
		return new Win32SolidBrush(color);
	}
	ShitGraph::Pen* Win32GraphicDevice::CreatePen(const Color& color, Scalar width) {
		return new Win32Pen(color, width);
	}
}
#endif