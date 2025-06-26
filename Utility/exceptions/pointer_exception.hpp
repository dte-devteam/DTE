#pragma once
#include "exception.hpp"
namespace dte_utils {
	struct bad_weak_ptr : exception {
		bad_weak_ptr(size_t code = 0) noexcept;
		bad_weak_ptr(const bad_weak_ptr& other) noexcept;
		bad_weak_ptr& operator=(const bad_weak_ptr&) = delete;
	};
}