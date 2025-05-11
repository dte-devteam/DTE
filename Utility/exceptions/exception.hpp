#pragma once
namespace dte_utils {
	struct exception {
		protected:
			size_t _error_code;
			const char* _what;
		public:
			exception(size_t code = 0) noexcept : _error_code(code), _what(nullptr) {}
			template<size_t N>
			exception(size_t code, const char(&what)[N]) noexcept : _error_code(code), _what(what) {}
			exception(const exception& ex) noexcept : _error_code(ex._error_code), _what(ex._what) {}
			const char* what() const {
				return _what ? _what : "Unknown error";
			}
			size_t error_code() const {
				return _error_code;
			}
			exception& operator=(const exception& ex) noexcept {
				if (this == &ex) {
					return *this;
				}
				_error_code = ex._error_code;
				_what = ex._what;
				return *this;
			}
	};
}