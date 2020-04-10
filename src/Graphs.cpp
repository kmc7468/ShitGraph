#include <ShitGraph/Graphs.hpp>

#include <cmath>
#include <utility>

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

	ImplicitFunctionClass Ellipse(double x, double y, double xRadius, double yRadius) {
		ImplicitFunctionClass graphClass;
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
}