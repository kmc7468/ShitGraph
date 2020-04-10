#pragma once

#include <ShitGraph/Function.hpp>

#include <vector>

namespace ShitGraph {
	ExplicitFunctionClass Polynomial(std::vector<Scalar> coefs);
	ExplicitFunctionGraph* CreatePolynomial(std::vector<Scalar> coefs);
	ImplicitFunctionClass Ellipse(double x, double y, double xRadius, double yRadius);
	ImplicitFunctionGraph* CreateEllipse(double x, double y, double xRadius, double yRadius);

	ExplicitFunctionClass CFunction(Scalar(*function)(Scalar));
	ExplicitFunctionClass CFunction(Scalar(*function)(Scalar), CheckContinuityFunction checkContinuity);
	ExplicitFunctionGraph* CreateCFunction(Scalar(*function)(Scalar));
	ExplicitFunctionGraph* CreateCFunction(Scalar(*function)(Scalar), CheckContinuityFunction checkContinuity);
}