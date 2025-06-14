#pragma once
#include "unit.hpp"

#include <iostream>
namespace dte_token {
	struct stream;
	struct dte_function {
		struct metadata {
			dte_utils::dynamic_cstring _name;
			/*
			later add metadata for complex function building
			*/
			metadata& operator=(const metadata& other);
			metadata& operator=(metadata&& other) noexcept;
		};
		struct step {
			//some data can be compressed to unit attributes (but its slower) 
			unit data;
			size_t delta_frame;
			size_t delta_jump;
			bool is_executable;
		};
		//protected:
			metadata _meta;
			dte_utils::dynamic_array<step> _steps;
		public:
			//dte_function(metadata&& meta,  const dte_utils::dynamic_array<step> steps = {});
			//dte_function(const dte_function& other);
			//dte_function(dte_function&& other);
			const metadata& get_meta() const;
			dte_function& operator=(const dte_function& other);
			dte_function& operator=(dte_function&& other) noexcept;
			size_t operator()(stream& s, size_t frame_offset);
	};
}