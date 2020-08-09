#include <ShitGraph/Expression.hpp>

#include <cassert>

namespace ShitGraph {
	Variable::Variable(char character) noexcept
		: Character(character) {}
	Variable::Variable(char character, Variable* subscript) noexcept
		: Character(character), Subscript(subscript) {
		assert(this != subscript);
	}
	Variable::~Variable() {
		delete Subscript;
	}
}