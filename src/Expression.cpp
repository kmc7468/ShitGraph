#include <ShitGraph/Expression.hpp>

#include <algorithm>
#include <cassert>

namespace ShitGraph {
	Term::~Term() {}
}

namespace ShitGraph {
	Expression::~Expression() {}
}

namespace ShitGraph {
	NumberTerm::NumberTerm(Scalar value) noexcept
		: Value(value) {}
}

namespace ShitGraph {
	VariableTerm::VariableTerm(char character) noexcept
		: Character(character) {}
	VariableTerm::VariableTerm(char character, Term* subscript) noexcept
		: Character(character), Subscript(subscript) {
		assert(this != subscript);
	}
	VariableTerm::~VariableTerm() {
		delete Subscript;
	}
}

namespace ShitGraph {
	FractionTerm::FractionTerm(Terms* numerator, Terms* denominator) noexcept
		: Numerator(numerator), Denominator(denominator) {}
	FractionTerm::~FractionTerm() {
		delete Numerator;
		delete Denominator;
	}
}

namespace ShitGraph {
	ParenthesesTerm::ParenthesesTerm(Terms* expression) noexcept
		: Expression(expression) {}
	ParenthesesTerm::~ParenthesesTerm() {
		delete Expression;
	}
}

namespace ShitGraph {
	SignTerm::SignTerm(ShitGraph::Term* term) noexcept
		: Term(term) {}
	SignTerm::SignTerm(bool isNegative, ShitGraph::Term* term) noexcept
		: IsNegative(isNegative), Term(term) {}
	SignTerm::~SignTerm() {
		delete Term;
	}
}

namespace ShitGraph {
	MultiplicationTerm::MultiplicationTerm(Term* left, Term* right) noexcept
		: Left(left), Right(right) {}
	MultiplicationTerm::MultiplicationTerm(Term* left, Term* right, bool hasDot) noexcept
		: Left(left), Right(right), HasDot(hasDot) {}
}

namespace ShitGraph {
	ExponentiationTerm::ExponentiationTerm(Term* base, Term* exponent) noexcept
		: Base(base), Exponent(exponent) {}
	ExponentiationTerm::~ExponentiationTerm() {
		delete Base;
		delete Exponent;
	}
}

namespace ShitGraph {
	Terms::~Terms() {
		for (Term* term : m_Terms) {
			delete term;
		}
	}

	void Terms::AddTerm(Term* term) {
		m_Terms.push_back(term);
	}
	void Terms::RemoveTerm(Term* term) {
		const auto iter = std::find(m_Terms.begin(), m_Terms.end(), term);
		assert(iter != m_Terms.end());
		m_Terms.erase(iter);
	}
}