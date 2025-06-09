#pragma once
#include "memory/dynamic_string.hpp"
#include "data_stack.hpp"
struct c_function {
	struct metadata {
		dte_utils::dynamic_cstring name;
		/*
		later add metadata for complex function building
		*/
	};
	typedef size_t(*func)(data_stack&, size_t);//TODO
	//protected:
		func _body;
		metadata _meta;
	public:
		//c_function(func body, const metadata& meta);
		const metadata& get_meta() const;
		size_t operator()(data_stack& stack, size_t frame_offset) const;
};