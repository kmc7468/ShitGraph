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
	graphs.AddGraph(CreateEllipse(0, 0, 1, 1)->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction(std::sin)->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction(std::cos)->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction(std::log2)->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction([](Scalar x) {
		return std::pow(2, x);
	})->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction([](Scalar x) {
		return std::pow(0.5, x);
	})->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateCFunction([](Scalar x) {
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
	graphs.AddGraph(new MultivaluedExplicitFunctionGraph(fClass));

	graphs.AddGraph(CreateCFunction([](Scalar x) {
		Scalar y = 0.0;
		for (int i = 0; i < 10; ++i) {
			y += std::pow(0.5, i) * std::cos(std::pow(5, i) * M_PI * x);
		}
		return y;
	})->ChangeColor(RandomColor()));

	graphs.AddGraph(CreateCFunction(std::tan, [](const Point& from, const Point& to) {
		return from.Y < to.Y;
	})->ChangeColor(RandomColor()));

	Win32Application application;
	application.Initialize();

	Win32Window mainWindow(renderer, instance, L"ShitGraph");
	mainWindow.Show(cmdShow);

	return application.Run();
}