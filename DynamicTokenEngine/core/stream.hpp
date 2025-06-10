#pragma once
#include "data_stack.hpp"
struct dte_function;
struct stream {
	data_stack stack;
	//we can store simple pointers because functions are hold by functions/stream/global
	dte_utils::dynamic_stack<dte_function*> call_stack;
};