#pragma once
//#include "unit.hpp"
#include "memory/dynamic_string.hpp"
#include "pointer/strong_ref.hpp"
#include "types.hpp"
#include "semi_pointer.hpp"
namespace dte_module {
	struct c_func_unit;
}
namespace dte_token {
	struct stream;
	struct c_function;
	struct dte_function {
		struct metadata {
			dte_utils::dynamic_cstring name;
			size_t max_jump;
		};
		struct step {
			union function_unit {
				function_unit() {}
				function_unit(const dte_utils::atomic_weak_ref<const c_function>& c_func);
				function_unit(const dte_utils::atomic_strong_ref<dte_function>& dte_func);
				~function_unit() {}
				dte_utils::atomic_weak_ref<const c_function> c_func;
				dte_utils::atomic_strong_ref<dte_function> dte_func;
			};
			protected:
				function_unit _func_unit;
				bool _is_dynamic;
				dte_utils::dynamic_array<size_t> _jumps;
				semi_pointer _semi_ptr;
			public:
				step(const dte_utils::atomic_weak_ref<const c_function>& c_func, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp);
				step(const dte_utils::atomic_weak_ref<const c_function>& c_func, dte_utils::dynamic_array<size_t>&& jumps, const semi_pointer& sp);
				step(const dte_utils::atomic_strong_ref<dte_function>& dte_func, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp);
				step(const dte_utils::atomic_strong_ref<dte_function>& dte_func, dte_utils::dynamic_array<size_t>&& jumps, const semi_pointer& sp);
				step(const step& other);
				~step();
				const function_unit& get_function_unit() const noexcept;
				bool get_is_dynamic() const noexcept;
				const dte_utils::dynamic_array<size_t>& get_jumps() const noexcept;
				const semi_pointer& get_semi_ptr() const noexcept;
		};
		//protected:
			metadata _meta;
			dte_utils::dynamic_array<step> _steps;
		public:
			//dte_function(metadata&& meta, dte_utils::dynamic_array<step>&& steps) : _meta(std::move(meta)), _steps(std::move(steps)) {}
			//dte_function(const dte_function& other) = delete;
			//dte_function(dte_function&& other);	//TODO
			const metadata& get_meta() const noexcept;
			dte_function& operator=(const dte_function& other);
			dte_function& operator=(dte_function&& other) noexcept;
			size_t operator()(stream& s, size_t frame_offset);
			~dte_function();
	};
}