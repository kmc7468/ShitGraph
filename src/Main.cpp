#define _USE_MATH_DEFINES

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graph.hpp>
#include <ShitGraph/Graphs.hpp>
#include <ShitGraph/platform/win32/Gdiplus.hpp>
#include <ShitGraph/platform/win32/Graphic.hpp>
#include <ShitGraph/platform/win32/Render.hpp>

#include <cmath>
#include <Windows.h>

// Constants
constexpr ShitGraph::Scalar INITIALLY_SCALE = 0.262144;
constexpr ShitGraph::Scalar MAGNIFICATION = 1 / 0.75;

// Variables
ShitGraph::Graphs g_Graphs(INITIALLY_SCALE);

POINT g_MousePos;
bool g_IsMoving = false;

// Function declarations
RECT GetClientRectangle(HWND handle);

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

void Paint(HWND handle);
void LButtonDown(LPARAM lParam);
void LButtonUp();
void MouseMove(HWND handle, LPARAM lParam);
void MouseWheel(HWND handle, WPARAM	wParam);

// Function definitions
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmdShow) {
	using namespace ShitGraph;

	Win32StartupGdiplus();

	Win32Render renderer(instance);
	renderer.CreateMainWindow(WndProc);

	g_Graphs.AddGraph(CreateXAxis());
	g_Graphs.AddGraph(CreateYAxis());

	g_Graphs.AddGraph(CreatePolynomial({ -6, 11, -6, 1 })->ChangeColor(RandomColor()));
	g_Graphs.AddGraph(CreateEllipse(0, 0, 1, 1)->ChangeColor(RandomColor()));
	g_Graphs.AddGraph(CreateCFunction(std::sin)->ChangeColor(RandomColor()));
	g_Graphs.AddGraph(CreateCFunction(std::cos)->ChangeColor(RandomColor()));
	g_Graphs.AddGraph(CreateCFunction(std::log2)->ChangeColor(RandomColor()));
	g_Graphs.AddGraph(CreateCFunction([](Scalar x) {
		return std::pow(2, x);
	})->ChangeColor(RandomColor()));
	g_Graphs.AddGraph(CreateCFunction([](Scalar x) {
		return std::pow(0.5, x);
	})->ChangeColor(RandomColor()));

	MultivaluedImplicitFunctionClass fClass;
	fClass.CheckContinuity = ContinuousFunction;
	fClass.Parameter = nullptr;
	fClass.Color = RandomColor();
	fClass.Function = [](const FunctionParameter*, Scalar x, Vector& y) {
		const Scalar sqrt = std::sqrt(4 * 3 * x);
		if (std::isnormal(sqrt) || sqrt == 0) {
			y.push_back(sqrt);
			y.push_back(-sqrt);
		}
	};
	g_Graphs.AddGraph(new MultivaluedExplicitFunctionGraph(fClass));

	g_Graphs.AddGraph(CreateCFunction([](Scalar x) {
		Scalar y = 0.0;
		for (int i = 0; i < 10; ++i) {
			y += std::pow(0.5, i) * std::cos(std::pow(5, i) * M_PI * x);
		}
		return y;
	})->ChangeColor(RandomColor()));

	g_Graphs.AddGraph(CreateCFunction(std::tan, [](const Point& from, const Point& to) {
		return from.Y < to.Y;
	})->ChangeColor(RandomColor()));

	/*Guarder*/

	const int result = renderer.Run(cmdShow);
	Win32ShutdownGdiplus();

	return result;
}

RECT GetClientRectangle(HWND handle) {
	RECT clientRectApi;
	GetClientRect(handle, &clientRectApi);
	return clientRectApi;
}

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT: return Paint(handle), 0;
	case WM_LBUTTONDOWN: return LButtonDown(lParam), 0;
	case WM_LBUTTONUP: return LButtonUp(), 0;
	case WM_MOUSEMOVE: return MouseMove(handle, lParam), 0;
	case WM_MOUSEWHEEL: return MouseWheel(handle, wParam), 0;
	case WM_DESTROY: return PostQuitMessage(0), 0;
	}
	return DefWindowProc(handle, message, wParam, lParam);
}

void Paint(HWND handle) {
	PAINTSTRUCT ps;
	const HDC dc = BeginPaint(handle, &ps);

	const RECT clientRectApi = GetClientRectangle(handle);
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
}
void LButtonDown(LPARAM lParam) {
	g_MousePos.x = LOWORD(lParam);
	g_MousePos.y = HIWORD(lParam);
	g_IsMoving = true;
}
void LButtonUp() {
	g_IsMoving = false;
}
void MouseMove(HWND handle, LPARAM lParam) {
	const WORD x = LOWORD(lParam), y = HIWORD(lParam);
	if (g_IsMoving) {
		const ShitGraph::Point center = g_Graphs.GetCenter();

		g_Graphs.SetCenter({
			center.X - (g_MousePos.x - x) * g_Graphs.GetScale(),
			center.Y + (g_MousePos.y - y) * g_Graphs.GetScale()
		});
		InvalidateRect(handle, nullptr, false);
	}
	g_MousePos.x = x;
	g_MousePos.y = y;
}
void MouseWheel(HWND handle, WPARAM	wParam) {
	const RECT clientRectApi = GetClientRectangle(handle);
	const ShitGraph::Point mouse = g_Graphs.Logical(clientRectApi.right, clientRectApi.bottom,
		{ static_cast<ShitGraph::Scalar>(g_MousePos.x), static_cast<ShitGraph::Scalar>(g_MousePos.y) });

	const ShitGraph::Scalar delta = static_cast<SHORT>(HIWORD(wParam)) > 0 ? 1 / MAGNIFICATION : MAGNIFICATION;
	g_Graphs.SetScale(g_Graphs.GetScale() * delta);

	if (!(LOWORD(wParam) & MK_CONTROL)) {
		const ShitGraph::Point newMouse = g_Graphs.Logical(clientRectApi.right, clientRectApi.bottom,
			{ static_cast<ShitGraph::Scalar>(g_MousePos.x), static_cast<ShitGraph::Scalar>(g_MousePos.y) });
		const ShitGraph::Point center = g_Graphs.GetCenter();
		g_Graphs.SetCenter({
			center.X + (newMouse.X - mouse.X) / delta,
			center.Y + (newMouse.Y - mouse.Y) / delta,
		});
	}

	InvalidateRect(handle, nullptr, false);
}