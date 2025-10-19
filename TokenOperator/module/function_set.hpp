#pragma once
#include "token/c_function.hpp"
namespace dte_module {
	struct function_set {
		protected:
			const dte_token::c_function* _pointer;
			size_t _num;
		public:
			constexpr function_set() : _pointer(nullptr), _num(0) {}
			template<size_t N>
			constexpr function_set(const dte_token::c_function(&set)[N]) : _pointer(set), _num(N) {}
			constexpr function_set(const function_set& set) : _pointer(set.get_pointer()), _num(set.get_num()) {}
			constexpr const dte_token::c_function* get_pointer() const noexcept {
				return _pointer;
			}
			constexpr size_t get_num() const noexcept {
				return _num;
			}
	};
}