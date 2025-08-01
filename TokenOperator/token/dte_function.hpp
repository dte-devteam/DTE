#pragma once
//#include "unit.hpp"
#include "memory/dynamic_string.hpp"
#include "pointer/strong_ref.hpp"
#include "c_function.hpp"
#include "types.hpp"
#include "semi_pointer.hpp"
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
			//some data can be compressed to unit attributes (but its slower) 
			union G {
				G() {}
				~G() {}
				dte_utils::strong_ref<c_function> cf;
				dte_utils::strong_ref<dte_function> df;
			} fu;
			bool is_dynamic;
			dte_utils::dynamic_array<size_t> jumps;
			semi_pointer sp;
			step(const dte_utils::strong_ref<c_function>& cf, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp) :
				is_dynamic(false), jumps(jumps), sp(sp) {
				new (&fu.cf) dte_utils::strong_ref<c_function>(cf);
			}
			step(const dte_utils::strong_ref<dte_function>& df, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp) :
				is_dynamic(true), jumps(jumps), sp(sp) {
				new (&fu.df) dte_utils::strong_ref<dte_function>(df);
			}
			step(const step& other) : is_dynamic(other.is_dynamic), sp(other.sp), jumps(other.jumps) {
				if (is_dynamic) {
					new (&fu.df) dte_utils::strong_ref<dte_function>(other.fu.df);
				}
				else {
					new (&fu.cf) dte_utils::strong_ref<c_function>(other.fu.cf);
				}
			}
			~step() {
				if (is_dynamic) {
					fu.df.~strong_ref();
				}
				else {
					fu.cf.~strong_ref();
				}
			}
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