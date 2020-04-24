#include <ShitGraph/platform/win32/Graphic.hpp>

#ifdef SHITGRAPH_WIN32
#include <vector>

namespace {
	std::wstring WideChar(const std::string_view& multiByte) {
		const int length = MultiByteToWideChar(CP_ACP, 0, multiByte.data(), static_cast<int>(multiByte.size()), nullptr, 0);
		std::wstring result(length, 0);
		MultiByteToWideChar(CP_ACP, 0, multiByte.data(), static_cast<int>(multiByte.size()), result.data(), length);
		return result;
	}
}

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
	Win32Font::Win32Font(std::string name, Scalar size)
		: Font(std::move(name), size) {
		const std::wstring nameWide = WideChar(GetName());
		m_Handle = new Gdiplus::Font(nameWide.c_str(), static_cast<Gdiplus::REAL>(size));
	}
	Win32Font::~Win32Font() {
		delete m_Handle;
	}

	void* Win32Font::GetHandle() const noexcept {
		return const_cast<decltype(m_Handle)>(m_Handle);
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
	void Win32GraphicDevice::DrawPoint(const ShitGraph::Brush* brush, const Point& point, Scalar radius) {
		const Gdiplus::REAL width = static_cast<Gdiplus::REAL>(radius * 2);
		m_Graphics.FillEllipse(static_cast<Gdiplus::Brush*>(brush->GetHandle()),
			static_cast<Gdiplus::REAL>(point.X - radius), static_cast<Gdiplus::REAL>(point.Y - radius), width, width);
	}
	void Win32GraphicDevice::DrawString(const ShitGraph::Font* font, const ShitGraph::Brush* brush, const Point& location, const std::string& string) {
		const std::wstring stringWide = WideChar(string);
		m_Graphics.DrawString(stringWide.c_str(), static_cast<INT>(stringWide.size()), static_cast<Gdiplus::Font*>(font->GetHandle()),
			GdiplusObject(location), static_cast<Gdiplus::Brush*>(brush->GetHandle()));
	}

#undef CreateFont

	ShitGraph::SolidBrush* Win32GraphicDevice::AllocateSolidBrush(const Color& color) {
		return new Win32SolidBrush(color);
	}
	ShitGraph::Pen* Win32GraphicDevice::AllocatePen(const Color& color, Scalar width) {
		return new Win32Pen(color, width);
	}
	ShitGraph::Font* Win32GraphicDevice::AllocateFont(std::string name, Scalar size) {
		return new Win32Font(std::move(name), size);
	}
}
#endif