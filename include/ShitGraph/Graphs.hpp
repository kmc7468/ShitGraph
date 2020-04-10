#pragma once

#include <ShitGraph/Function.hpp>

#include <vector>

namespace ShitGraph {
	ExplicitFunctionClass Polynomial(std::vector<Scalar> coefs);
	ImplicitFunctionClass Ellipse(double x, double y, double xRadius, double yRadius);
}