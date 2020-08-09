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
}