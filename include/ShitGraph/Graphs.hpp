#pragma once

#include <ShitGraph/Function.hpp>

#include <vector>

namespace ShitGraph {
	ExplicitFunctionClass XAxis();
	ExplicitFunctionClass YAxis();
	Graph* CreateXAxis();
	Graph* CreateYAxis();

	ExplicitFunctionClass Polynomial(std::vector<Scalar> coefs);
	ExplicitFunctionGraph* CreatePolynomial(std::vector<Scalar> coefs);
	MultivaluedImplicitFunctionClass Ellipse(double x, double y, double xRadius, double yRadius);
	MultivaluedExplicitFunctionGraph* CreateEllipse(double x, double y, double xRadius, double yRadius);

	ExplicitFunctionClass CFunction(Scalar(*function)(Scalar));
	ExplicitFunctionClass CFunction(Scalar(*function)(Scalar), CheckContinuityFunction checkContinuity);
	ExplicitFunctionGraph* CreateCFunction(Scalar(*function)(Scalar));
	ExplicitFunctionGraph* CreateCFunction(Scalar(*function)(Scalar), CheckContinuityFunction checkContinuity);
}