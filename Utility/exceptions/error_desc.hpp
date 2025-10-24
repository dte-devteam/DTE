#pragma once
namespace dte_error {
	constexpr inline char* STACK_CODE[] = {
		"Attempt to corrupt stack header",	//STACK_HEADER_DAMAGE
		"Attempt to get object from stack with unexpected size"	//STACK_GET_UNEXPECTED_SIZE
	};
	constexpr inline char* FUNCTION_CODE[] = {
		"Attempt to select invalid jump (out of index)"	//FUNCTION_OVERJUMP
	};
}