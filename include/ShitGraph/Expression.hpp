#pragma once

#include <ShitGraph/CoreType.hpp>

#include <vector>

namespace ShitGraph {
	class Term {
	public:
		Term() noexcept = default;
		Term(const Term&) = delete;
		virtual ~Term() = 0;

	public:
		Term& operator=(const Term&) = delete;
	};

	class Expression {
	private:
		std::vector<Term*> m_Terms;

	public:
		Expression() noexcept = default;
		Expression(const Expression&) = delete;
		virtual ~Expression();

	public:
		Expression& operator=(const Expression&) = delete;

	public:
		void AddTerm(Term* term);
		void RemoveTerm(Term* term);
	};
}

namespace ShitGraph {
	class NumberTerm final : public Term {
	public:
		Scalar Value = 0;

	public:
		NumberTerm() noexcept = default;
		NumberTerm(Scalar value) noexcept;
		NumberTerm(const NumberTerm&) = delete;
		virtual ~NumberTerm() override = default;

	public:
		NumberTerm& operator=(const NumberTerm&) = delete;
	};

	class VariableTerm final : public Term {
	public:
		char Character = 0;
		Term* Subscript = nullptr;

	public:
		explicit VariableTerm(char character) noexcept;
		VariableTerm(char character, Term* subscript) noexcept;
		VariableTerm(const VariableTerm&) = delete;
		virtual ~VariableTerm() override;

	public:
		VariableTerm& operator=(const VariableTerm&) = delete;
	};

	class FractionTerm final : public Term {
	public:
		Expression* Numerator = nullptr;
		Expression* Denominator = nullptr;

	public:
		FractionTerm(Expression* numerator, Expression* denominator) noexcept;
		FractionTerm(const FractionTerm&) = delete;
		virtual ~FractionTerm() override;

	public:
		FractionTerm& operator=(const FractionTerm&) = delete;
	};

	class ParenthesesTerm final : public Term {
	public:
		ShitGraph::Expression* Expression = nullptr;

	public:
		ParenthesesTerm(ShitGraph::Expression* expression) noexcept;
		ParenthesesTerm(const ParenthesesTerm&) = delete;
		virtual ~ParenthesesTerm() override;

	public:
		ParenthesesTerm& operator=(const ParenthesesTerm&) = delete;
	};

	class SignTerm final : public Term {
	public:
		bool IsNegative = false;
		ShitGraph::Term* Term = nullptr;

	public:
		explicit SignTerm(ShitGraph::Term* term) noexcept;
		SignTerm(bool isNegative, ShitGraph::Term* term) noexcept;
		SignTerm(const SignTerm&) = delete;
		virtual ~SignTerm() override;

	public:
		SignTerm& operator=(const SignTerm&) = delete;
	};

	class MultiplicationTerm final : public Term {
	public:
		Term* Left = nullptr;
		Term* Right = nullptr;
		bool HasDot = false;

	public:
		MultiplicationTerm(Term* left, Term* right) noexcept;
		MultiplicationTerm(Term* left, Term* right, bool hasDot) noexcept;
		MultiplicationTerm(const MultiplicationTerm&) = delete;
		virtual ~MultiplicationTerm() override = default;

	public:
		MultiplicationTerm& operator=(const MultiplicationTerm&) = delete;
	};

	class ExponentiationTerm final : public Term {
	public:
		Term* Base = nullptr;
		Term* Exponent = nullptr;

	public:
		ExponentiationTerm(Term* base, Term* exponent) noexcept;
		ExponentiationTerm(const ExponentiationTerm&) = delete;
		virtual ~ExponentiationTerm() override;

	public:
		ExponentiationTerm& operator=(const ExponentiationTerm&) = delete;
	};
}