#pragma once
#include "unit.hpp"
struct c_function {
	struct step {
		func body;
	};
	//protected:
		dynamic_array<step> steps;
		/*
		later add metadata for complex function building
		*/
	public:
		size_t execute(function_stack& stack, size_t frame_offset) {
			size_t i = 0;
			while (i < steps.get_used()) {
				i += steps[i].body(stack, frame_offset);
			}
			return i - steps.get_used();
		}
};
struct dte_function {
	dynamic_array<dte_function*> steps;

	dte_function* execute(function_stack& stack, size_t frame_offset) {
		return nullptr; //TODO
	}
};