#include "mem_handler.hpp"
#include "memory.hpp"
#include <utility>
using namespace dte_utils;
mem_handler::mem_handler(size_type size, size_type alignment) : _ptr(aligned_xmalloc(size, alignment)) {}
mem_handler::mem_handler(mem_handler&& other) noexcept : _ptr(other._ptr) {
	other._ptr = nullptr; 
}
mem_handler::~mem_handler() { 
	aligned_free(_ptr);
}
mem_handler& mem_handler::operator=(mem_handler&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(_ptr, other._ptr);
	return *this;
}