#include <ShitGraph/platform/win32/Gdiplus.hpp>

#ifdef SHITGRAPH_WIN32
namespace ShitGraph {
	static ULONG_PTR Win32GdiplusToken;

	bool Win32StartupGdiplus() {
		Gdiplus::GdiplusStartupInput input;
		return Gdiplus::GdiplusStartup(&Win32GdiplusToken, &input, nullptr) == Gdiplus::Ok;
	}
	void Win32ShutdownGdiplus() {
		Gdiplus::GdiplusShutdown(Win32GdiplusToken);
	}
}
#endif