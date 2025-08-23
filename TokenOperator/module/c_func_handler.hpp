#pragma once
#include "memory/static_array.hpp"
#include "pointer/strong_ref.hpp"

#include "token/c_function.hpp"
//namespace dte_token {
//	struct c_function;
//}
namespace dte_module {

	//template<same_variadic<dte_token::c_function>... Args>

	

	struct c_func_handler {
		using size_type = size_t;
		//protected:
			const dte_utils::atomic_strong_ref<dte_token::c_function>* c_func_ptr;
			size_type c_func_num;
		//public:
			c_func_handler() noexcept : c_func_ptr(nullptr), c_func_num(0) {}
			template<size_type N>
			c_func_handler(const dte_utils::static_array<dte_utils::atomic_strong_ref<dte_token::c_function>, N>& functions) noexcept : c_func_ptr(functions.begin()), c_func_num(N) {}
	};
}