#include <ShitGraph/platform/win32/Window.hpp>

#ifdef SHITGRAPH_WIN32
#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/platform/win32/Graphic.hpp>

#include <stdexcept>

namespace ShitGraph {
	Win32Window::Win32Window(EventAdaptor* eventAdaptor, HINSTANCE instance, const wchar_t* title)
		: Window(eventAdaptor) {
		CreateHandle(instance, title);
	}
	Win32Window::~Win32Window() {
		DestroyWindow(m_Handle);
	}

	void Win32Window::Show() {
		ShowWindow(m_Handle, SW_SHOW);
	}
	void Win32Window::Hide() {
		ShowWindow(m_Handle, SW_HIDE);
	}

	void Win32Window::CreateHandle(HINSTANCE instance, const wchar_t* title) {
		static const wchar_t* className = CreateClass(instance);
		m_Handle = CreateWindow(className, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
			nullptr, nullptr, instance, nullptr);
		if (!m_Handle) throw std::runtime_error("Failed to create a window.");

		SetWindowLongPtr(m_Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}
	const wchar_t* Win32Window::CreateClass(HINSTANCE instance) {
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"Win32Window";
		wc.hIconSm = nullptr;

		if (RegisterClassEx(&wc) == 0) throw std::runtime_error("Failed to register a window class.");
		else return wc.lpszClassName;
	}
	LRESULT CALLBACK Win32Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
		const LONG_PTR longPtr = GetWindowLongPtr(handle, GWLP_USERDATA);
		Win32Window* const window = reinterpret_cast<Win32Window*>(longPtr);
		if (window) {
			window->AdaptEvent(message, wParam, lParam);
			return 0;
		} else return DefWindowProc(handle, message, wParam, lParam);
	}
	void Win32Window::AdaptEvent(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			const HDC dc = BeginPaint(m_Handle, &ps);
			const RECT clientRectApi = GetClientRect();
			const Rectangle clientRect = {
				{ static_cast<Scalar>(clientRectApi.left), static_cast<Scalar>(clientRectApi.top) },
				{ static_cast<Scalar>(clientRectApi.right), static_cast<Scalar>(clientRectApi.bottom) },
			};

			const HDC bufferDc = CreateCompatibleDC(dc);
			const HBITMAP bufferBitmap = CreateCompatibleBitmap(dc, clientRectApi.right, clientRectApi.bottom);
			const HBITMAP originalBitmap = static_cast<HBITMAP>(SelectObject(bufferDc, bufferBitmap));

			Win32GraphicDevice device(m_Handle, bufferDc, clientRect);
			Paint(device);

			BitBlt(dc, 0, 0, clientRectApi.right, clientRectApi.bottom, bufferDc, 0, 0, SRCCOPY);
			SelectObject(bufferDc, originalBitmap);
			DeleteObject(bufferBitmap);
			DeleteDC(bufferDc);

			EndPaint(m_Handle, &ps);
			break;
		}
		case WM_DESTROY: Destroy(); break;

		case WM_LBUTTONDOWN: MouseDown(LOWORD(lParam), HIWORD(lParam), MouseButton::Left); break;
		case WM_LBUTTONUP: MouseUp(LOWORD(lParam), HIWORD(lParam), MouseButton::Left); break;
		case WM_MOUSEMOVE: MouseMove(LOWORD(lParam), HIWORD(lParam)); break;
		case WM_MOUSEWHEEL: MouseWheel(static_cast<SHORT>(HIWORD(wParam))); break;

		case WM_KEYDOWN: KeyDown(static_cast<int>(wParam)); break;
		}
	}
	RECT Win32Window::GetClientRect() const noexcept {
		RECT rect;
		::GetClientRect(m_Handle, &rect);
		return rect;
	}
}

namespace ShitGraph {
	int Win32Application::Run() {
		MSG message;
		while (GetMessage(&message, nullptr, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		return static_cast<int>(message.wParam);
	}
}
#endif