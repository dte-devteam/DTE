#include "exception.hpp"
using namespace dte_utils;
exception::exception(size_t code) noexcept : _error_code(code), _what(nullptr) {}
exception::exception(const exception& other) noexcept : _error_code(other._error_code), _what(other._what) {}
const char* exception::what() const noexcept {
	return _what ? _what : "Unknown error";
}
size_t exception::error_code() const noexcept {
	return _error_code;
}
exception& exception::operator=(const exception& other) noexcept {
	if (this == &other) {
		return *this;
	}
	_error_code = other._error_code;
	_what = other._what;
	return *this;
}