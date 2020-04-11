#define _USE_MATH_DEFINES

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graph.hpp>
#include <ShitGraph/Graphs.hpp>
#include <ShitGraph/platform/win32/Gdiplus.hpp>
#include <ShitGraph/platform/win32/Graphic.hpp>
#include <ShitGraph/platform/win32/Render.hpp>

#include <cmath>
#include <Windows.h>

ShitGraph::Graphs g_Graphs;

POINT g_MousePos;
bool g_IsMoving = false;

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmdShow) {
	using namespace ShitGraph;

	Win32StartupGdiplus();

	Win32Render renderer(instance);
	renderer.CreateMainWindow(WndProc);

	g_Graphs.AddGraph(CreateXAxis());
	g_Graphs.AddGraph(CreateYAxis());

	g_Graphs.AddGraph(MakeForY(CreatePolynomial({ -6, 11, -6, 1 })));
	g_Graphs.AddGraph(CreateEllipse(0, 0, 1, 1));
	g_Graphs.AddGraph(CreateCFunction(std::sin));
	g_Graphs.AddGraph(CreateCFunction(std::cos));
	/*g_Graphs.AddGraph(CreateCFunction(std::tan, [](const Point& from, const Point& to) {
		const Scalar aInt = from.X - std::fmod(from.X, M_PI / 2);
		const Scalar bInt = to.X - std::fmod(to.X, M_PI / 2);
		return aInt == bInt;
	}));*/

	const int result = renderer.Run(cmdShow);
	Win32ShutdownGdiplus();

	return result;
}

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		const HDC dc = BeginPaint(handle, &ps);

		RECT clientRectApi;
		GetClientRect(handle, &clientRectApi);
		const ShitGraph::Rectangle clientRect = {
			{ static_cast<ShitGraph::Scalar>(clientRectApi.left), static_cast<ShitGraph::Scalar>(clientRectApi.top) },
			{ static_cast<ShitGraph::Scalar>(clientRectApi.right), static_cast<ShitGraph::Scalar>(clientRectApi.bottom) }
		};

		const HDC bufferDC = CreateCompatibleDC(dc);
		const HBITMAP bitmap = CreateCompatibleBitmap(dc, clientRectApi.right, clientRectApi.bottom);
		const HGDIOBJ oldBitmap = SelectObject(bufferDC, bitmap);

		ShitGraph::Win32GraphicDevice device(handle, bufferDC, clientRect);
		g_Graphs.Render(device);

		BitBlt(dc, 0, 0, clientRectApi.right, clientRectApi.bottom, bufferDC, 0, 0, SRCCOPY);
		SelectObject(bufferDC, oldBitmap);
		DeleteObject(bitmap);
		DeleteDC(bufferDC);

		EndPaint(handle, &ps);
		return 0;
	}

	case WM_LBUTTONDOWN:
		g_MousePos.x = LOWORD(lParam);
		g_MousePos.y = HIWORD(lParam);
		g_IsMoving = true;
		return 0;

	case WM_MOUSEMOVE:
		if (g_IsMoving) {
			const ShitGraph::Point center = g_Graphs.GetCenter();
			const WORD x = LOWORD(lParam), y = HIWORD(lParam);

			g_Graphs.SetCenter({
				center.X - (g_MousePos.x - x) * g_Graphs.GetScale(),
				center.Y + (g_MousePos.y - y) * g_Graphs.GetScale()
			});
			g_MousePos.x = x;
			g_MousePos.y = y;

			InvalidateRect(handle, nullptr, true);
		}
		return 0;

	case WM_LBUTTONUP:
		g_IsMoving = false;
		return 0;

	case WM_MOUSEWHEEL:
		if (static_cast<SHORT>(HIWORD(wParam)) > 0) {
			g_Graphs.SetScale(g_Graphs.GetScale() / 2);
		} else {
			g_Graphs.SetScale(g_Graphs.GetScale() * 2);
		}
		InvalidateRect(handle, nullptr, true);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(handle, message, wParam, lParam);
}