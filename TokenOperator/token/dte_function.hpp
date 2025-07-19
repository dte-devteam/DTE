#pragma once
#include "unit.hpp"
#include "types.hpp"
namespace dte_token {
	struct stream;
	struct d_action {
		constructor* constr;
		destructor* destr;
		size_t size;
		void* data;
	};
	struct dte_function {
		struct metadata {
			dte_utils::dynamic_cstring name;
			size_t max_jump;
			/*
			later add metadata for complex function building
			*/
		};
		struct step {
			//some data can be compressed to unit attributes (but its slower) 
			unit data;
			size_t delta_frame;
			bool is_executable;
			dte_utils::dynamic_array<size_t> jumps;

			d_action act;
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