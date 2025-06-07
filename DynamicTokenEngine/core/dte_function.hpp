#pragma once
#include "c_function.hpp"
#include "unit.hpp"
struct stream;
struct dte_function {
	struct step {
		//some data can be compressed to unit attributes (but its slower) 
		unit data;
		size_t delta_frame;
		size_t delta_jump;
		bool is_executable;
	};
	//protected:
		dte_utils::dynamic_array<step> steps;


	public:
		size_t operator()(stream& s, size_t frame_offset);
};