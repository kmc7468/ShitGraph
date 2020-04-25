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

	/*graphs.AddGraph(CreateXAxis());
	graphs.AddGraph(CreateYAxis());*/

	//graphs.AddGraph(CreatePolynomial({ -6, 11, -6, 1 })->ChangeColor(RandomColor()));
	graphs.AddGraph(CreateEllipse(0, 0, 2, 2)->ChangeColor(RandomColor()));
	/*graphs.AddGraph(CreateCFunction(std::sin)->ChangeColor(RandomColor()));
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
	})->ChangeColor(RandomColor()));*/

	//graphs.AddGraph(CreatePolynomial({ 1, 1 })->ChangeColor(RandomColor()));

	ImplicitFunctionClass fClass;
	fClass.CheckContinuity = ContinuousFunction;
	fClass.Parameter = nullptr;
	fClass.Color = RandomColor();
	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X, 2) + std::pow(point.Y, 2) - 1;
		//return std::sin(point.X) - std::cos(point.Y);
		//return point.Y - point.X;
		//return 1 / (point.Y - 1) - 1;
		//return point.Y * std::sin(point.X) + point.X * std::cos(point.Y) - 1;
		//return std::sqrt(std::pow(point.X, 2) + std::pow(point.Y, 2)) - std::atan(point.Y / point.X);
		//return std::pow(point.X, 3) + 3 * point.X - std::pow(point.Y, 2) +3;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	/*fClass.CheckContinuity = [](const Point& from, const Point& to) {
		return from.Y < to.Y;
	};*/
	fClass.Color = RandomColor();
	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return point.Y - std::tan(point.X);
	};
	//graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	/* Guarder */

	Win32Application application;
	application.Initialize();

	Win32Window mainWindow(renderer, instance, L"ShitGraph");
	mainWindow.Show(cmdShow);

	return application.Run();
}