#pragma once
#include "exception.hpp"
namespace dte_utils {
	struct invalid_range : exception {
		invalid_range(size_t code = 0) noexcept;
		invalid_range(const invalid_range& other) noexcept;
		invalid_range& operator=(const invalid_range&) = delete;
	};
	struct out_of_range : exception {
		out_of_range(size_t code = 0) noexcept;
		out_of_range(const out_of_range& other) noexcept;
		out_of_range& operator=(const out_of_range&) = delete;
	};
	struct zero_size_access : exception {
		zero_size_access(size_t code = 0) noexcept;
		zero_size_access(const zero_size_access& other) noexcept;
		zero_size_access& operator=(const zero_size_access&) = delete;
	};
}