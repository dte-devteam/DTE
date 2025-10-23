#pragma once
namespace dte_utils {
	//DOES NOT HANDLE const char* what LIFETIME! 
	struct exception {
		protected:
			size_t _error_code;
			const char* _what;
		public:
			exception(size_t code = 0, const char* what = nullptr) noexcept;
			exception(const exception& other) noexcept;
			const char* what() const noexcept;
			size_t error_code() const noexcept;
			exception& operator=(const exception& other) noexcept;
	};
}