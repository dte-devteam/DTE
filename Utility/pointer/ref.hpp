#pragma once
#include "memory/constraints.hpp"
#include "function_traits.hpp"
namespace dte_utils {
	template<typename T>
	using ref_pointer = std::conditional_t<
		return_type_v<T>,
		T,
		T*
	>;

	struct ref_counter {
		using size_type = size_t;
		size_type weak_owners;
		size_type strong_owners;
	};
}