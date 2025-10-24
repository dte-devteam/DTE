#pragma once
#include "data_stack.hpp"
namespace dte_token {
	struct dte_function;
	struct stream {
		size_t functional_index;
		data_stack stack;
		//we can store simple pointers because functions are hold by functions/stream/global
		dte_utils::dynamic_stack<const dte_function*> call_stack;
	};
}