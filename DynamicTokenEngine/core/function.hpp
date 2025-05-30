#pragma once
#include "unit.hpp"
struct c_function {
	protected:
		func _body;
		/*
		later add metadata for complex function building
		*/
	public:
		size_t operator()(function_stack& stack, size_t frame_offset) {
			return _body(stack, frame_offset);
		}
};
struct dte_function {
	dynamic_array<unit> steps;

	dte_function* execute(function_stack& stack, size_t frame_offset) {
		if (true) {//execute

		}
		else {	//add value to stack

		}
		return nullptr; //TODO
	}
};