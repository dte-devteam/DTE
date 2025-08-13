#pragma once
namespace dte_token {
	struct c_function;
}
namespace dte_module {

	//template<same_variadic<dte_token::c_function>... Args>



	struct module_instance {
		dte_token::c_function* c_func_array_ptr;
		size_t num_c_func;

	};
	
}