#pragma once
#include "unit.hpp"
struct stream;
struct dte_function {
	struct metadata {
		dte_utils::dynamic_cstring name;
		/*
		later add metadata for complex function building
		*/
	};
	struct step {
		//some data can be compressed to unit attributes (but its slower) 
		unit data;
		size_t delta_frame;
		size_t delta_jump;
		bool is_executable;
	};
	//protected:
		dte_utils::dynamic_array<step> _steps;
		metadata _meta;

	public:
		size_t operator()(stream& s, size_t frame_offset);
};