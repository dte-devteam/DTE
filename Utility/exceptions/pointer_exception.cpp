#include "pointer_exception.hpp"
using namespace dte_utils;
bad_weak_ptr::bad_weak_ptr(size_t code) noexcept : exception(code, "bad weak pointer: data is not handled") {}
bad_weak_ptr::bad_weak_ptr(const bad_weak_ptr& other) noexcept : exception(other) {}