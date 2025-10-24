#pragma once
namespace dte_error {
	constexpr inline size_t section_size = 1000;
	constexpr inline size_t full_error_code(size_t section, size_t size) {
		return section_size * section + size;
	}
	namespace error_section {
		enum CODE : size_t {
			FUNCTION,
			STACK
		};
	}
	namespace stack {
		enum CODE : size_t {
			STACK_HEADER_DAMAGE,	//Attempt to corrupt stack header
			STACK_GET_UNEXPECTED_SIZE	//Attempt to get object from stack with unexpected size
		};
	}
	namespace function {
		enum CODE : size_t {
			FUNCTION_OVERJUMP	//Attempt to select invalid jump (out of index)
		};
	}
}