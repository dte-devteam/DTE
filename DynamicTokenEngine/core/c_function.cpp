#include "c_function.hpp"
using namespace dte_utils;
const dynamic_cstring& c_function::get_name() const {
	return name;
}
size_t c_function::operator()(data_stack& stack, size_t frame_offset) {
	return _body(stack, frame_offset);
}