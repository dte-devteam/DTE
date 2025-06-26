#include "logic_exception.hpp"
using namespace dte_utils;
invalid_range::invalid_range(size_t code) noexcept : exception(code, "invalid range") {}
invalid_range::invalid_range(const invalid_range& other) noexcept : exception(other) {}

out_of_range::out_of_range(size_t code) noexcept : exception(code, "out of range") {}
out_of_range::out_of_range(const out_of_range& other) noexcept : exception(other) {}

zero_size_access::zero_size_access(size_t code) noexcept : exception(code, "zero size access") {}
zero_size_access::zero_size_access(const zero_size_access& other) noexcept : exception(other) {}