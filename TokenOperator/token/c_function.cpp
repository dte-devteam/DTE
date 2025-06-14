#include "c_function.hpp"
#include "data_stack.hpp"
using namespace dte_utils;
using namespace dte_token;
c_function::c_function(func body, const metadata& meta) : _body(body), _meta(meta) {}
c_function::c_function(func body, metadata&& meta) : _body(body), _meta(std::move(meta)) {}
c_function::c_function(c_function&& other) noexcept : _body(other._body), _meta(std::move(other._meta)) {}
const c_function::metadata& c_function::get_meta() const {
	return _meta;
}
const c_function::func c_function::get_body() const {
	return _body;
}
size_t c_function::operator()(data_stack& stack, size_t frame_offset) const {
	return _body(stack, frame_offset);
}