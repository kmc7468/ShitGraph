#include <ShitGraph/platform/win32/Render.hpp>

#ifdef SHITGRAPH_WIN32
namespace ShitGraph {
	Win32Render::Win32Render(HINSTANCE instance) noexcept
		: m_Instance(instance) {}

	bool Win32Render::CreateMainWindow(WNDPROC wndProc) noexcept {
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = wndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_Instance;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"GraphMain";
		wc.hIconSm = nullptr;

		if (RegisterClassEx(&wc) == 0) return false;
		else if ((m_MainWindow = CreateWindow(L"GraphMain", L"ShitGraph", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
			nullptr, nullptr, m_Instance, nullptr)) == nullptr) return false;
		else return true;
	}
	int Win32Render::Run(int cmdShow) {
		ShowWindow(m_MainWindow, cmdShow);

		MSG message;
		while (GetMessage(&message, nullptr, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		return static_cast<int>(message.wParam);
	}
}
#endif