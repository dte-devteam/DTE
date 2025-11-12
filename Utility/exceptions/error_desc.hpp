#pragma once
namespace dte_error::desc {
	//STACK_CODE
	constexpr inline char STACK_HEADER_DAMAGE[] = "Attempt to corrupt stack header";
	constexpr inline char STACK_GET_UNEXPECTED_SIZE[] = "Attempt to get object from stack with unexpected size";
	//FUNCTION_CODE
	constexpr inline char FUNCTION_OVERJUMP[] = "Attempt to select invalid jump (out of index)";
}