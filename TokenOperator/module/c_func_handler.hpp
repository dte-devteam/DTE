#pragma once
#include "memory/static_array.hpp"
#include "c_func_unit.hpp"
namespace dte_module {
	struct c_func_handler {
		using size_type = size_t;
		//protected:
			const c_func_unit* c_func_ptr;
			size_type c_func_num;
		//public:
			c_func_handler() noexcept : c_func_ptr(nullptr), c_func_num(0) {}
			template<size_type N>
			c_func_handler(const dte_utils::static_array<c_func_unit, N>& functions) noexcept : c_func_ptr(functions.begin()), c_func_num(N) {}
			const c_func_unit& operator[](size_type index) {
				if (index < N) {
					return c_func_ptr[index];
				}
				throw dte_utils::out_of_range();
			}
	};
}