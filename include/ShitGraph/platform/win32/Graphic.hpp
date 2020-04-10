#pragma once
#include <ShitGraph/Predefined.hpp>

#ifdef SHITGRAPH_WIN32
#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graphic.hpp>
#include <ShitGraph/platform/win32/Gdiplus.hpp>

#include <cstddef>

namespace ShitGraph {
	class Win32SolidBrush final : public SolidBrush {
	private:
		Gdiplus::SolidBrush m_Handle;

	public:
		Win32SolidBrush(const Color& color);
		Win32SolidBrush(const Win32SolidBrush&) = delete;
		virtual ~Win32SolidBrush() override = default;

	public:
		Win32SolidBrush& operator=(const Win32SolidBrush&) = delete;

	public:
		virtual void* GetHandle() const noexcept override;
	};

	class Win32Pen final : public Pen {
	private:
		Win32SolidBrush m_Brush;
		Gdiplus::Pen m_Handle;

	public:
		Win32Pen(const Color& color);
		Win32Pen(const Color& color, Scalar width);
		Win32Pen(const Win32Pen&) = delete;
		virtual ~Win32Pen() override = default;

	public:
		Win32Pen& operator=(const Win32Pen&) = delete;

	public:
		virtual void* GetHandle() const noexcept override;
	};
}

namespace ShitGraph {
	class Win32GraphicDevice final : public GraphicDevice {
	private:
		HWND m_Window;
		Gdiplus::Graphics m_Graphics;

	public:
		Win32GraphicDevice(HWND window, HDC dc, const ShitGraph::Rectangle& rectangle);
		Win32GraphicDevice(const Win32GraphicDevice&) = delete;
		virtual ~Win32GraphicDevice() = default;

	public:
		Win32GraphicDevice& operator=(const Win32GraphicDevice&) = delete;

	public:
		virtual void DrawLines(const ShitGraph::Pen* pen, const Point* points, std::size_t size) override;

	protected:
		virtual ShitGraph::SolidBrush* CreateSolidBrush(const Color& color) override;
		virtual ShitGraph::Pen* CreatePen(const Color& color, Scalar width) override;
	};
}
#endif