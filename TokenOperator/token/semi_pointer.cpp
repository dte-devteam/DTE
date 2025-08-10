#include "semi_pointer.hpp"
using namespace dte_token;
semi_pointer::data::data(void* ptr) noexcept : ptr(ptr) {}
semi_pointer::data::data(size_t offset) noexcept : offset(offset) {}

semi_pointer::semi_pointer(void* ptr) noexcept : _spu(ptr), _real_ptr(true) {}
semi_pointer::semi_pointer(size_t offset) noexcept : _spu(offset), _real_ptr(false) {}
semi_pointer::semi_pointer(const semi_pointer& other) noexcept : _real_ptr(other.is_real_ptr()) {
	if (is_real_ptr()) {
		_spu.ptr = other.get_spu().ptr;
	}
	else {
		_spu.offset = other.get_spu().offset;
	}
}
bool semi_pointer::is_real_ptr() const noexcept {
	return _real_ptr;
}
const semi_pointer::data& semi_pointer::get_spu() const noexcept {
	return _spu;
}
semi_pointer& semi_pointer::operator=(void* ptr) noexcept {
	_spu.ptr = ptr;
	_real_ptr = true;
	return *this;
}
semi_pointer& semi_pointer::operator=(size_t offset) noexcept {
	_spu.offset = offset;
	_real_ptr = false;
	return *this;
}
semi_pointer& semi_pointer::operator=(const semi_pointer& other) noexcept {
	if (other.is_real_ptr()) {
		_spu.ptr = other.get_spu().ptr;
	}
	else {
		_spu.offset = other.get_spu().offset;
	}
	return *this;
}