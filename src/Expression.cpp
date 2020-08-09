#include <ShitGraph/Expression.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

namespace ShitGraph {
	Expression::~Expression() {}
}

namespace ShitGraph {
	NumberTerm::NumberTerm(Scalar value) noexcept
		: Value(value) {}

	Scalar NumberTerm::Evaluate(const VariableTable&) noexcept {
		return Value;
	}
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

	Scalar VariableTerm::Evaluate(const VariableTable& table) noexcept {
		return table.at(this);
	}
}

namespace ShitGraph {
	FractionTerm::FractionTerm(Terms* numerator, Terms* denominator) noexcept
		: Numerator(numerator), Denominator(denominator) {}
	FractionTerm::~FractionTerm() {
		delete Numerator;
		delete Denominator;
	}

	Scalar FractionTerm::Evaluate(const VariableTable& table) noexcept {
		return Numerator->Evaluate(table) / Denominator->Evaluate(table);
	}
}

namespace ShitGraph {
	ParenthesesTerm::ParenthesesTerm(Terms* expression) noexcept
		: Expression(expression) {}
	ParenthesesTerm::~ParenthesesTerm() {
		delete Expression;
	}

	Scalar ParenthesesTerm::Evaluate(const VariableTable& table) noexcept {
		return Expression->Evaluate(table);
	}
}

namespace ShitGraph {
	SignTerm::SignTerm(ShitGraph::Term* term) noexcept
		: Expression(term) {}
	SignTerm::SignTerm(bool isNegative, ShitGraph::Term* term) noexcept
		: IsNegative(isNegative), Expression(term) {}
	SignTerm::~SignTerm() {
		delete Expression;
	}

	Scalar SignTerm::Evaluate(const VariableTable& table) noexcept {
		const Scalar evaluated = Expression->Evaluate(table);
		return IsNegative ? -evaluated : evaluated;
	}
}

namespace ShitGraph {
	MultiplicationTerm::MultiplicationTerm(Term* left, Term* right) noexcept
		: Left(left), Right(right) {}
	MultiplicationTerm::MultiplicationTerm(Term* left, Term* right, bool hasDot) noexcept
		: Left(left), Right(right), HasDot(hasDot) {}

	Scalar MultiplicationTerm::Evaluate(const VariableTable& table) noexcept {
		return Left->Evaluate(table) * Right->Evaluate(table);
	}
}

namespace ShitGraph {
	ExponentiationTerm::ExponentiationTerm(Term* base, Term* exponent) noexcept
		: Base(base), Exponent(exponent) {}
	ExponentiationTerm::~ExponentiationTerm() {
		delete Base;
		delete Exponent;
	}

	Scalar ExponentiationTerm::Evaluate(const VariableTable& table) noexcept {
		return std::pow(Base->Evaluate(table), Exponent->Evaluate(table));
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

	Scalar Terms::Evaluate(const VariableTable& table) {
		Scalar sum = 0;
		for (Term* term : m_Terms) {
			sum += term->Evaluate(table);
		}
		return sum;
	}
}

namespace ShitGraph {
	Equality::Equality(Terms* left, Terms* right) noexcept
		: Left(left), Right(right) {}
	Equality::~Equality() {}
}