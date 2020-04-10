#pragma once
#include <ShitGraph/Predefined.hpp>

#ifdef SHITGRAPH_WIN32
#include <Windows.h>

namespace ShitGraph {
	class Win32Render final {
	private:
		HINSTANCE m_Instance = nullptr;
		HWND m_MainWindow = nullptr;

	public:
		Win32Render(HINSTANCE instance) noexcept;
		Win32Render(const Win32Render&) = delete;
		~Win32Render() = default;

	public:
		Win32Render& operator=(const Win32Render&) = delete;

	public:
		bool CreateMainWindow(WNDPROC wndProc) noexcept;
		int Run(int cmdShow);
	};
}
#endif