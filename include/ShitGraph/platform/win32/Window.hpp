#pragma once
#include <ShitGraph/Predefined.hpp>

#ifdef SHITGRAPH_WIN32
#include <ShitGraph/Window.hpp>

#include <Windows.h>

namespace ShitGraph {
	class Win32Window final : public Window {
	private:
		HWND m_Handle = nullptr;

	public:
		explicit Win32Window(EventAdaptor* eventAdaptor, HINSTANCE instance, const wchar_t* title);
		Win32Window(const Win32Window&) = delete;
		virtual ~Win32Window() override;

	public:
		Win32Window& operator=(const Win32Window&) = delete;

	private:
		void CreateHandle(HINSTANCE instance, const wchar_t* title);
		static const wchar_t* CreateClass(HINSTANCE instance);
		static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
		void AdaptEvent(UINT message, WPARAM wParam, LPARAM lParam);
		RECT GetClientRect() const noexcept;
	};
}
#endif