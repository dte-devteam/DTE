#pragma once
#include "memory/static_array.hpp"
#include "c_func_unit.hpp"
namespace dte_module {
	struct c_func_handler {
		using size_type = size_t;
		protected:
			const dte_token::c_function* c_func_ptr;
			const size_type c_func_num;
		public:
			constexpr c_func_handler() noexcept : c_func_ptr(nullptr), c_func_num(0) {}
			template<size_type N>
			constexpr c_func_handler(const dte_token::c_function (&functions)[N]) noexcept : c_func_ptr(functions), c_func_num(N) {}
			const dte_token::c_function* begin() const noexcept {
				return c_func_ptr;
			}
			const dte_token::c_function* end() const noexcept {
				return c_func_ptr + c_func_num;
			}
			const size_type get_num() const noexcept {
				return c_func_num;
			}
	};
}