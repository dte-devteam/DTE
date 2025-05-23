#pragma once
#include "unit.hpp"
struct c_function {
	//protected:
		dynamic_array<func> body;
		/*
		later add metadata for complex function building
		*/
	public:
		size_t execute(function_stack& stack, size_t frame_offset) {
			size_t i = 0;
			while (i < body.get_used()) {
				i += body[i](stack, frame_offset) + 1;
			}
			return i - body.get_used();
		}
};
struct dte_function {
	union action {
		unit u;
		dte_function* df;
	};
	action a;
	void execute(function_stack& stack, size_t frame_offset) {
		
	}
};