#pragma once
#include "stack_functions.hpp"
namespace dte_core {
	template<typename T>
	concept addable = requires(T & target, const T & source) {
		target += source;
	};


	template<addable T>
	inline size_t add(dte_token::data_stack& ds, const dte_token::semi_pointer::data& spd) {
		*get<T>(ds, spd.offset) += *get<const T>(ds, spd.offset + 1);
		return 0;
	}
}