#pragma once
namespace dte_error::code {
	enum STACK_CODE : size_t {
		STACK_HEADER_DAMAGE,			//Attempt to corrupt stack header
		STACK_GET_UNEXPECTED_SIZE		//Attempt to get object from stack with unexpected size
	};
	enum FUNCTION_CODE : size_t {
		FUNCTION_OVERJUMP	//Attempt to select invalid jump (out of index)
	};
}