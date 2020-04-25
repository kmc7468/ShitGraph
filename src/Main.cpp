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

	const Color yellow{ 244, 220, 38 };
	const Color red{ 233, 41, 41 };
	const Color pink{ 255, 105, 180 };

	ImplicitFunctionClass fClass;
	fClass.CheckContinuity = ContinuousFunction;
	fClass.Color = red;
	fClass.Parameter = nullptr;
	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X - 55, 2) + std::pow(point.Y - 54, 2) - point.Y - 131;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X + 51, 2) + std::pow(point.Y - 61.9, 2) - point.Y - 129;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-26 < x && x < -9.65) return 65;
		else return INFINITY;
	}));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (15.65 < x && x < 30.02) return 65;
		else return INFINITY;
	}));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-9.65 < x && x < 15.65) return -0.05 * std::pow(x - 3, 2) + 73;
		else return INFINITY;
	}));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-156.5 < x && x < -59) return std::pow(1.019, -x) + 0.01 * x + 102;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-156.5 < x && x < -37.1) return -0.0028 * std::pow(x + 82, 2) + 135;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-121.2 < x && x < -99.1) return 0.029 * std::pow(x + 92, 2) + 106;
		else return INFINITY;
	}));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-40 < x && x < 44) return (-1 / 159.0) * std::pow(x, 2) + 138;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (42.033 < x && x < 149.741) return -3000 * x / (x - 1) + 3200;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (64.01 < x && x < 149.741) return -50 * x / (x - 238) + 95;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (104.3 < x && x < 114.6) return 0.09 * std::pow(x - 88, 2) + 110;
		else return INFINITY;
	}));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-55.99 < x && x < -38) return 0.00598 * std::pow(x - 22, 2);
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (36.5 < x && x < 67.26) return -12 * x / (x - 90) + 8;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		const Scalar y = 0.017 * std::pow(x - 63.5, 2) - 68.5;
		if (y < -56) return y;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (87.14 < x && x < 104.5) return -59;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (43.49 < x && x < 89.4) return -0.008 * std::pow(x - 62, 2) + 70;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (89.37 < x && x < 113.394) return 64.007596;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		const Scalar y = 0.076 * std::pow(x - 2, 2) + 32;
		if (y < 65) return y;
		else return INFINITY;
	})->ChangeColor(pink));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		const Scalar y = -0.045 * std::pow(x - 2, 2) + 62;
		if (y > 50.84) return y;
		else return INFINITY;
	})->ChangeColor(pink));

	fClass.Color = {};
	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X - 38, 2) + 0.8 * std::pow(point.Y - 90, 2) - point.Y - 45;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X + 32, 2) + 0.79 * std::pow(point.Y - 94, 2) - point.Y - 45;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X - 39, 2) + 0.5 * std::pow(point.Y - 96, 2) - point.Y + 80;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X + 30.8, 2) + 0.5* std::pow(point.Y - 99.2, 2) - point.Y + 80;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	fClass.Function = [](const FunctionParameter*, const Point& point) {
		return std::pow(point.X - 2.7, 2) + std::pow(point.Y - 83, 2) - point.Y + 81;
	};
	graphs.AddGraph(new ImplicitFunctionGraph(fClass));

	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		const Scalar y = -5 * x + 300;
		if (130.79762422 < y && y < 164.265) return y;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (27.16 < x && x < 74.88) return -0.001 * std::pow(x, 2) + 165;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (124.204 < x && x < 188.15) return -0.001 * std::pow(x, 2) + 165;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		const Scalar y = 1.3 * x - 115;
		if (54.2 < y && y < 129.66) return y;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (67.6 < x && x < 130.18) return -0.0027 * std::pow(x, 2) + 100;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (67.6 < x && x < 87.76) return -0.013 * std::pow(x, 2) + 147;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (71.8 < x && x < 87.8) return 0.5 * x + 3;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (4.31 < x && x < 91.5) return 0.006 * std::pow(x - 72, 2) - 96;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-81.09 < x && x < -66.8) return 17 * x / (x + 94) + 50;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-61.93 < x && x < -59) return std::pow(-1.14, x + 70) - 96;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-82.53 < x && x < -60) return std::pow(1.3, -(x + 70)) - 99;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-113.87 < x && x < -98.85) return 0.08 * std::pow(x + 112, 2) - 76.5;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-72.32 < x && x < -56.75) return 0.006 * std::pow(x + 72.26, 2) - 82.53;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-93.72 < x && x < -84) return -0.09 * std::pow(x + 84, 2) + 100;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-84 < x && x < -63.94) return -0.05 * std::pow(x + 84, 2) + 100;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (64.84 < x && x < 127.85) return -0.007 * std::pow(x - 68, 2) + 39;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (120.91 < x && x < 127.85) return 0.5 * x - 50;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (120.91 < x && x < 126.59) return -0.6 * x + 83;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (64.4 < x && x < 126.59) return -0.8 * x / (x - 135) - 5;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-62.68 < x && x < -3.6) return 0.014 * std::pow(x + 36, 2) + 71;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (63 < x && x < 71.81) return std::pow(1.28, -(x - 78)) - 33;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-46 < x && x < 55.8) return 0.001 * std::pow(x - 7, 2) - 101;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (69.6 < x && x < 83.91) return -x / (x - 87) - 100;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-72.86 < x && x < -62.68) return -0.02 * std::pow(x + 75, 2) + 84;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-76.2 < x && x < -69) return 0.06 * std::pow(x + 69, 2) - 117;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-69 < x && x < -38.9) return 0.02 * std::pow(x + 69, 2) - 117;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (52.55 < x && x < 83.904) return 0.02 * std::pow(x - 80, 2) - 114;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (72.933 < x && x < 83.926) return std::pow(1.11, -(x - 99)) - 110;
		else return INFINITY;
	})->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-110 < x && x < -105) return -0.175 * std::pow(x + 110, 2) + 88;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));
	graphs.AddGraph(CreateCFunction([](Scalar x) -> Scalar {
		if (-113.7 < x && x < -110) return -0.3 * std::pow(x + 110, 2) + 88;
		else return INFINITY;
	})->MakeForY()->ChangeColor(yellow));

	/* Guarder */

	Win32Application application;
	application.Initialize();

	Win32Window mainWindow(renderer, instance, L"ShitGraph");
	mainWindow.Show(cmdShow);

	return application.Run();
}