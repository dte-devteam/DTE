#include "memory_exception.hpp"
using namespace dte_utils;
bad_malloc::bad_malloc(size_t code) noexcept : exception(code, "bad allocation") {}
bad_malloc::bad_malloc(const bad_malloc& other) noexcept : exception(other) {}

bad_realloc::bad_realloc(size_t code) noexcept : exception(code, "bad reallocation") {}
bad_realloc::bad_realloc(const bad_realloc& other) noexcept : exception(other) {}

nullptr_access::nullptr_access(size_t code) noexcept : exception(code, "nullptr access") {}
nullptr_access::nullptr_access(const nullptr_access& other) noexcept : exception(other) {}