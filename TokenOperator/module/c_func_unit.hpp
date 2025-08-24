#pragma once
#include "pointer/weak_ref.hpp"
#include "token/c_function.hpp"
namespace dte_module {
	struct c_func_unit {
		using size_type = size_t;
		protected:
			const dte_utils::atomic_weak_ref<const dte_token::c_function> _c_func_ptr;
			const dte_token::c_function _c_func;
		public:
			c_func_unit(dte_token::c_function::func* body, const dte_token::c_function::metadata& meta, destructor* args_destructor = nullptr);
			c_func_unit(dte_token::c_function::func* body, dte_token::c_function::metadata&& meta, destructor* args_destructor = nullptr);
			const dte_utils::atomic_weak_ref<const dte_token::c_function>& get_c_func_ptr() const noexcept;
	};
}