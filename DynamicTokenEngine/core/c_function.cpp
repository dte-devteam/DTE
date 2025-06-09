#include "c_function.hpp"
using namespace dte_utils;
//c_function::c_function(func body, const metadata& meta) : _body(body), _meta(meta) {}
const c_function::metadata& c_function::get_meta() const {
	return _meta;
}
size_t c_function::operator()(data_stack& stack, size_t frame_offset) const {
	return _body(stack, frame_offset);
}