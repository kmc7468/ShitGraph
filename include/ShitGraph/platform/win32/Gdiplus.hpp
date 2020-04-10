#pragma once
#include <ShitGraph/Predefined.hpp>

#ifdef SHITGRAPH_WIN32
#include <Windows.h>
#include <gdiplus.h>

namespace ShitGraph {
	bool Win32StartupGdiplus();
	void Win32ShutdownGdiplus();
}
#endif