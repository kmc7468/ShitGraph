#include <ShitGraph/Expression.hpp>

#include <algorithm>
#include <cassert>

namespace ShitGraph {
	Term::~Term() {}
}

namespace ShitGraph {
	Expression::~Expression() {
		for (Term* term : m_Terms) {
			delete term;
		}
	}

	void Expression::AddTerm(Term* term) {
		m_Terms.push_back(term);
	}
	void Expression::RemoveTerm(Term* term) {
		const auto iter = std::find(m_Terms.begin(), m_Terms.end(), term);
		assert(iter != m_Terms.end());
		m_Terms.erase(iter);
	}
}

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