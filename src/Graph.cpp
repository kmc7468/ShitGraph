#include <ShitGraph/Graph.hpp>

namespace ShitGraph {
	Vector Graph::Solve(Scalar x) const {
		Vector y;
		Solve(x, y);
		return y;
	}
}