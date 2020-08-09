#pragma once

namespace ShitGraph {
	class Variable final {
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