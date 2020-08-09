#define _USE_MATH_DEFINES

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graph.hpp>
#include <ShitGraph/Graphs.hpp>
#include <ShitGraph/platform/win32/Window.hpp>

#include <cmath>
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmdShow) {
	using namespace ShitGraph;

	Renderer* renderer = new Renderer;
	Graphs& graphs = renderer->GetGraphs();

	graphs.AddGraph(CreateXAxis());
	graphs.AddGraph(CreateYAxis());

	graphs.AddGraph(CreatePolynomial({ -6, 11, -6, 1 })->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction(std::sin)->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction(std::cos)->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction(std::log2)->ChangeColor(RandomColor()));

	Win32Application application;
	application.Initialize();

	Win32Window mainWindow(renderer, instance, L"ShitGraph");
	mainWindow.Show(cmdShow);

	return application.Run();
}