#pragma once
namespace dte_utils {
	struct exception {
	protected:
		size_t _error_code;
		const char* _what;
	public:
		exception(size_t code = 0) noexcept;
		template<size_t N>
		exception(size_t code, const char(&what)[N]) noexcept : _error_code(code), _what(what) {}
		exception(const exception& other) noexcept;
		const char* what() const;
		size_t error_code() const;
		exception& operator=(const exception& other) noexcept;
	};
}