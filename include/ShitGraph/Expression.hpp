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

namespace ShitGraph::terms {
	class Number final : public Term {
	public:
		Scalar Value = 0;

	public:
		Number() noexcept = default;
		Number(Scalar value) noexcept;
		Number(const Number&) = delete;
		virtual ~Number() override = default;

	public:
		Number& operator=(const Number&) = delete;
	};

	class Variable final : public Term {
	public:
		char Character = 0;
		Term* Subscript = nullptr;

	public:
		explicit Variable(char character) noexcept;
		Variable(char character, Term* subscript) noexcept;
		Variable(const Variable&) = delete;
		virtual ~Variable() override;

	public:
		Variable& operator=(const Variable&) = delete;
	};

	class Fraction final : public Term {
	public:
		Expression* Numerator = nullptr;
		Expression* Denominator = nullptr;

	public:
		Fraction(Expression* numerator, Expression* denominator) noexcept;
		Fraction(const Fraction&) = delete;
		virtual ~Fraction() override;

	public:
		Fraction& operator=(const Fraction&) = delete;
	};

	class Parentheses final : public Term {
	public:
		ShitGraph::Expression* Expression = nullptr;

	public:
		Parentheses(ShitGraph::Expression* expression) noexcept;
		Parentheses(const Parentheses&) = delete;
		virtual ~Parentheses() override;

	public:
		Parentheses& operator=(const Parentheses&) = delete;
	};

	class Sign final : public Term {
	public:
		bool IsNegative = false;
		ShitGraph::Term* Term = nullptr;

	public:
		explicit Sign(ShitGraph::Term* term) noexcept;
		Sign(bool isNegative, ShitGraph::Term* term) noexcept;
		Sign(const Sign&) = delete;
		virtual ~Sign() override;

	public:
		Sign& operator=(const Sign&) = delete;
	};

	class Multiplication final : public Term {
	public:
		Term* Left = nullptr;
		Term* Right = nullptr;
		bool HasDot = false;

	public:
		Multiplication(Term* left, Term* right) noexcept;
		Multiplication(Term* left, Term* right, bool hasDot) noexcept;
		Multiplication(const Multiplication&) = delete;
		virtual ~Multiplication() override = default;

	public:
		Multiplication& operator=(const Multiplication&) = delete;
	};

	class Exponentiation final : public Term {
	public:
		Term* Base = nullptr;
		Term* Exponent = nullptr;

	public:
		Exponentiation(Term* base, Term* exponent) noexcept;
		Exponentiation(const Exponentiation&) = delete;
		virtual ~Exponentiation() override;

	public:
		Exponentiation& operator=(const Exponentiation&) = delete;
	};
}