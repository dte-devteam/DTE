#pragma once
#include "memory/dynamic_string.hpp"
#include "data_stack.hpp"
struct c_function {
	typedef size_t(*func)(data_stack&, size_t);//TODO
	//protected:
		func _body;
		dte_utils::dynamic_cstring name;
		/*
		later add metadata for complex function building
		*/
	public:
		const dte_utils::dynamic_cstring& get_name() const;
		size_t operator()(data_stack& stack, size_t frame_offset);
};