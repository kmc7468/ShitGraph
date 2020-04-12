#define _USE_MATH_DEFINES

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graph.hpp>
#include <ShitGraph/Graphs.hpp>
#include <ShitGraph/platform/win32/Gdiplus.hpp>
#include <ShitGraph/platform/win32/Graphic.hpp>
#include <ShitGraph/platform/win32/Render.hpp>

#include <chrono>
#include <cmath>
#include <optional>
#include <string>
#include <Windows.h>

// Constants
constexpr ShitGraph::Scalar INITIALLY_SCALE = 0.01002259575;
constexpr ShitGraph::Scalar MAGNIFICATION = 1 / 0.75;

// Variables
ShitGraph::Graphs g_Graphs(INITIALLY_SCALE);
std::optional<std::size_t> g_CurrentIndex;
bool g_OriginalVisible = true;

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
void KeyDown(HWND handle, WPARAM wParam);

void UpKey(HWND handle);
void DownKey(HWND handle);
void LeftKey(HWND handle);
void RightKey(HWND handle);

void Select(std::size_t index);
void Unselect(std::size_t index);

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
	g_Graphs.AddGraph(CreateCFunction([](Scalar x){
		return std::pow(x, x);
	})->ChangeColor(RandomColor()));

	MultivaluedExplicitFunctionClass fClass;
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
	case WM_KEYDOWN: return KeyDown(handle, wParam), 0;
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

	const ShitGraph::ManagedGraphicObject<ShitGraph::Brush> blackBrush(device, device.SolidBrush({ 0, 0, 0 }));
	const ShitGraph::ManagedGraphicObject<ShitGraph::Font> fpsFont(device, device.Font("Arial", 12));

	const auto start = std::chrono::system_clock::now();
	g_Graphs.Render(device);
	const auto end = std::chrono::system_clock::now();

	const std::chrono::duration<double, std::milli> milli = end - start;
	const int fps = static_cast<int>(1000 / milli.count());
	device.DrawString(fpsFont, blackBrush, { 0, 0 }, std::to_string(fps) + " fps");
	if (g_CurrentIndex) {
		device.DrawString(fpsFont, blackBrush, { 0, 20 }, "Graph: " + std::to_string(*g_CurrentIndex + 1) + '/' + std::to_string(g_Graphs.GetGraphCount()));
		device.DrawString(fpsFont, blackBrush, { 0, 40 }, g_OriginalVisible ? "Visible" : "Invisible");
	}

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
			center.X + (newMouse.X - mouse.X),
			center.Y + (newMouse.Y - mouse.Y),
		});
	}

	InvalidateRect(handle, nullptr, false);
}
void KeyDown(HWND handle, WPARAM wParam) {
	switch (wParam) {
	case 'O':
		g_Graphs.SetCenter({ 0, 0 });
		InvalidateRect(handle, nullptr, false);
		break;

	case 'R':
		g_Graphs.SetScale(INITIALLY_SCALE);
		InvalidateRect(handle, nullptr, false);
		break;

	case VK_ESCAPE:
		if (g_CurrentIndex) {
			Unselect(*g_CurrentIndex);
			g_CurrentIndex.reset();
			InvalidateRect(handle, nullptr, false);
		}
		break;

	case VK_UP: UpKey(handle); break;
	case VK_DOWN: DownKey(handle); break;
	case VK_LEFT: LeftKey(handle); break;
	case VK_RIGHT: RightKey(handle); break;
	}
}

void UpKey(HWND handle) {
	if (g_CurrentIndex && !g_OriginalVisible) {
		g_OriginalVisible = true;
		InvalidateRect(handle, nullptr, false);
	}
}
void DownKey(HWND handle) {
	if (g_CurrentIndex && g_OriginalVisible) {
		g_OriginalVisible = false;
		InvalidateRect(handle, nullptr, false);
	}
}
void LeftKey(HWND handle) {
	if (!g_CurrentIndex && g_Graphs.GetGraphCount()) {
		const auto index = g_Graphs.GetGraphCount() - 1;
		Select((g_CurrentIndex = index, index));
		InvalidateRect(handle, nullptr, false);
	} else if (g_CurrentIndex && *g_CurrentIndex) {
		Unselect(*g_CurrentIndex);
		Select(*g_CurrentIndex -= 1);
		InvalidateRect(handle, nullptr, false);
	}
}
void RightKey(HWND handle) {
	if (!g_CurrentIndex && g_Graphs.GetGraphCount()) {
		Select((g_CurrentIndex = 0, 0));
		InvalidateRect(handle, nullptr, false);
	} else if (g_CurrentIndex && g_CurrentIndex < g_Graphs.GetGraphCount() - 1) {
		Unselect(*g_CurrentIndex);
		Select(*g_CurrentIndex += 1);
		InvalidateRect(handle, nullptr, false);
	}
}

void Select(std::size_t index) {
	const auto graph = g_Graphs.GetGraph(index);
	graph->Width += 2;
	g_OriginalVisible = graph->Visible;
	graph->Visible = true;
}
void Unselect(std::size_t index) {
	const auto graph = g_Graphs.GetGraph(index);
	graph->Width -= 2;
	graph->Visible = g_OriginalVisible;
}