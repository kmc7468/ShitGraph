#pragma once

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
	class Variable final : public Term {
	public:
		char Character = 0;
		Variable* Subscript = nullptr;

	public:
		explicit Variable(char character) noexcept;
		Variable(char character, Variable* subscript) noexcept;
		Variable(const Variable&) = delete;
		~Variable();

	public:
		Variable& operator=(const Variable&) = delete;
	};
}