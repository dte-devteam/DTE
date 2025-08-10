#pragma once
//#include "unit.hpp"
#include "memory/dynamic_string.hpp"
#include "pointer/strong_ref.hpp"
#include "c_function.hpp"
#include "types.hpp"
#include "semi_pointer.hpp"
#include <iostream>
namespace dte_token {
	struct stream;
	struct dte_function {
		struct metadata {
			dte_utils::dynamic_cstring name;
			size_t max_jump;
			/*
			later add metadata for complex function building
			*/
		};
		struct step {
			union function_unit {
				function_unit() {}
				~function_unit() {}
				dte_utils::strong_ref<c_function> c_func;
				dte_utils::strong_ref<dte_function> dte_func;
			};
			protected:
				function_unit _func_unit;
				bool _is_dynamic;
				dte_utils::dynamic_array<size_t> _jumps;
				semi_pointer _semi_ptr;
				destructor* _imm_destructor;
			public:
				step(const dte_utils::strong_ref<c_function>& c_func, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp, destructor* imm_destructor);
				step(const dte_utils::strong_ref<dte_function>& dte_func, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp, destructor* imm_destructor);
				step(const step& other);
				~step();
				const function_unit& get_function_unit() const noexcept;
				bool get_is_dynamic() const noexcept;
				const dte_utils::dynamic_array<size_t>& get_jumps() const noexcept;
				const semi_pointer& get_semi_ptr() const noexcept;
				destructor* get_imm_destructor() const noexcept;
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
			~dte_function() {
				for (const step& s : _steps) {
					if (s.get_imm_destructor() && s.get_semi_ptr().is_real_ptr()) {
						s.get_imm_destructor()(s.get_semi_ptr().get_spu().ptr);
					}
				}
			}
	};
}