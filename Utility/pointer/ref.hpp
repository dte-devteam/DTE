#pragma once
#include "function_traits.hpp"
namespace dte_utils {
	struct ref_counter {
		using size_type = size_t;
		size_type weak_owners;
		size_type strong_owners;
	};
}