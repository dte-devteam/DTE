#include "mem_handler.hpp"
#include "memory.hpp"
using namespace dte_utils;
mem_handler::mem_handler(size_type size) : _ptr(xmalloc(size)) {}
mem_handler::mem_handler(pointer ptr) : _ptr(ptr) {}
mem_handler::mem_handler(mem_handler&& other) noexcept : _ptr(other) {
	other._ptr = nullptr; 
}
mem_handler::~mem_handler() { 
	free(_ptr);
}
void mem_handler::resize(size_type size) {
	_ptr = xrealloc(_ptr, size);
}
mem_handler& mem_handler::operator=(mem_handler&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(_ptr, other._ptr);
	return *this;
}
mem_handler::operator pointer() {
	return _ptr;
}
mem_handler::operator const_pointer() const {
	return _ptr;
}