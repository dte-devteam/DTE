#pragma once
#include "exception.hpp"
namespace dte_utils {
	struct bad_malloc : exception {
		bad_malloc(size_t code = 0) noexcept : exception(code, "bad allocation") {}
		bad_malloc(const bad_malloc& other) noexcept : exception(other) {}
		bad_malloc& operator=(const bad_malloc&) = delete;
	};
	struct bad_realloc : exception {
		bad_realloc(size_t code = 0) noexcept : exception(code, "bad reallocation") {}
		bad_realloc(const bad_realloc& other) noexcept : exception(other) {}
		bad_realloc& operator=(const bad_realloc&) = delete;
	};
	struct nullptr_access : exception {
		nullptr_access(size_t code = 0) noexcept : exception(code, "nullptr access") {}
		nullptr_access(const nullptr_access& other) noexcept : exception(other) {}
		nullptr_access& operator=(const nullptr_access&) = delete;
	};
}