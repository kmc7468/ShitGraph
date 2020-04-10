#include <Graph.hpp>

Vector Graph::Solve(Scalar x) const {
	Vector y;
	Solve(x, y);
	return y;
}