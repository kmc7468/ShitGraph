#include <ShitGraph/Graphs.hpp>

#include <cmath>
#include <utility>

namespace ShitGraph {
	ExplicitFunctionClass XAxis() {
		return ChangeWidth(Polynomial({ 0 }), 2.5);
	}
	ExplicitFunctionClass YAxis() {
		return MakeForY(XAxis());
	}
	Graph* CreateXAxis() {
		return new ExplicitFunctionGraph(XAxis());
	}
	Graph* CreateYAxis() {
		return new ExplicitFunctionGraph(YAxis());
	}
}

namespace ShitGraph {
	class PolynomialFunctionParameter final : public FunctionParameter {
	public:
		std::vector<Scalar> Coefficients;

	public:
		PolynomialFunctionParameter(std::vector<Scalar> coefs) noexcept
			: Coefficients(std::move(coefs)) {}
		PolynomialFunctionParameter(const PolynomialFunctionParameter&) = delete;
		virtual ~PolynomialFunctionParameter() override = default;

	public:
		PolynomialFunctionParameter& operator=(const PolynomialFunctionParameter&) = delete;
	};

	ExplicitFunctionClass Polynomial(std::vector<Scalar> coefs) {
		ExplicitFunctionClass functionClass;
		functionClass.Parameter = new PolynomialFunctionParameter(std::move(coefs));
		functionClass.CheckContinuity = ContinuousFunction;
		functionClass.Function = [](const FunctionParameter* parameter, Point& point) {
			const auto polyParameter = parameter->Cast<PolynomialFunctionParameter>();
			point.Y = 0;
			for (size_t i = 0; i < polyParameter->Coefficients.size(); ++i) {
				point.Y += polyParameter->Coefficients[i] * std::pow(point.X, i);
			}
			return true;
		};
		return functionClass;
	}
	ExplicitFunctionGraph* CreatePolynomial(std::vector<Scalar> coefs) {
		return new ExplicitFunctionGraph(Polynomial(std::move(coefs)));
	}
}

namespace ShitGraph {
	class EllipseParameter final : public FunctionParameter {
	public:
		double X = 0;
		double Y = 0;
		double XRadius = 0;
		double YRadius = 0;

	public:
		EllipseParameter(double x, double y, double xRadius, double yRadius)
			: X(x), Y(y), XRadius(xRadius), YRadius(yRadius) {}
		EllipseParameter(const EllipseParameter&) = delete;
		virtual ~EllipseParameter() override = default;

	public:
		EllipseParameter& operator=(const EllipseParameter&) = delete;
	};

	MultivaluedExplicitFunctionClass Ellipse(double x, double y, double xRadius, double yRadius) {
		MultivaluedExplicitFunctionClass graphClass;
		graphClass.Parameter = new EllipseParameter(x, y, xRadius, yRadius);
		graphClass.CheckContinuity = ContinuousFunction;
		graphClass.Function = [](const FunctionParameter* parameter, Scalar x, Vector& y) {
			const auto ellipseParameter = parameter->Cast<EllipseParameter>();

			const double sqrt = std::sqrt(std::pow(ellipseParameter->YRadius, 2) * (1 - std::pow(x - ellipseParameter->X, 2) / std::pow(ellipseParameter->XRadius, 2)));
			if (!std::isnormal(sqrt) && sqrt != 0) return;

			const double pSqrt = sqrt + ellipseParameter->Y;
			const double mSqrt = -sqrt + ellipseParameter->Y;

			y.push_back(pSqrt);
			y.push_back(mSqrt);
		};
		return graphClass;
	}
	MultivaluedExplicitFunctionGraph* CreateEllipse(double x, double y, double xRadius, double yRadius) {
		return new MultivaluedExplicitFunctionGraph(Ellipse(x, y, xRadius, yRadius));
	}
}

namespace ShitGraph {
	class CFunctionParameter final : public FunctionParameter {
	public:
		Scalar(*Function)(Scalar) = nullptr;

	public:
		CFunctionParameter(Scalar(*function)(Scalar)) noexcept
			: Function(function) {}
		CFunctionParameter(const CFunctionParameter&) = delete;
		virtual ~CFunctionParameter() override = default;

	public:
		CFunctionParameter& operator=(const CFunctionParameter&) = delete;
	};

	ExplicitFunctionClass CFunction(Scalar(*function)(Scalar)) {
		return CFunction(function, ContinuousFunction);
	}
	ExplicitFunctionClass CFunction(Scalar(*function)(Scalar), CheckContinuityFunction checkContinuity) {
		ExplicitFunctionClass graphClass;
		graphClass.Parameter = new CFunctionParameter(function);
		graphClass.CheckContinuity = checkContinuity;
		graphClass.Function = [](const FunctionParameter* parameter, Point& point) {
			const auto cfunctionParameter = parameter->Cast<CFunctionParameter>();
			point.Y = cfunctionParameter->Function(point.X);

			const bool isValid = std::isnormal(point.Y) || point.Y == 0;
			return isValid;
		};
		return graphClass;
	}
	ExplicitFunctionGraph* CreateCFunction(Scalar(*function)(Scalar)) {
		return new ExplicitFunctionGraph(CFunction(function));
	}
	ExplicitFunctionGraph* CreateCFunction(Scalar(*function)(Scalar), CheckContinuityFunction checkContinuity) {
		return new ExplicitFunctionGraph(CFunction(function, checkContinuity));
	}
}