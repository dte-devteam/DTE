#include "xmem_wrapper.hpp"
#include <utility>
using namespace dte_utils;
void xmem_wrapper::kill_value() {
	if (_destructor && _ptr) {
		_destructor(_ptr);
	}
}
xmem_wrapper::xmem_wrapper(pointer ptr, destructor destr_func) : _destructor(destr_func), mem_handler(ptr) {}
xmem_wrapper::~xmem_wrapper() {
	kill_value();
}

xmem_wrapper::xmem_wrapper(xmem_wrapper&& other) noexcept : _destructor(other._destructor), mem_handler(std::move(other)) {}

destructor xmem_wrapper::get_destructor() {
	return _destructor;
}
void xmem_wrapper::set_new_value(pointer ptr, destructor destr_func) {
	if (_ptr == ptr) {
		return;
	}
	kill_value();
	_ptr = ptr;
	_destructor = destr_func;
}

xmem_wrapper& xmem_wrapper::operator=(xmem_wrapper&& other) noexcept {
	mem_handler::operator=(std::move(other));
	std::swap(_destructor, other._destructor);
	return *this;
}