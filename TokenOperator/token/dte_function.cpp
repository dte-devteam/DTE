#include "dte_function.hpp"
#include "stream.hpp"
#include "c_function.hpp"
using namespace dte_utils;
using namespace dte_token;

//dte_function::dte_function(metadata meta, dte_utils::dynamic_array<step> steps) : _meta(meta),
//dte_function::dte_function(const dte_function& other);
//dte_function::dte_function(dte_function&& other);
const dte_function::metadata& dte_function::get_meta() const {
	return _meta;
}
dte_function& dte_function::operator=(const dte_function& other) {
	if (this == &other) {
		return *this;
	}

	return *this;
}
dte_function& dte_function::operator=(dte_function&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	_steps = std::move(other._steps);
	_meta = std::move(other._meta);
	return *this;
}
size_t dte_function::operator()(stream& s, size_t frame_offset) {
	s.call_stack.push_back(this);
	size_t i = 0;
	while (i < _steps.get_used()) {
		const step& action = _steps[i];
		size_t jump;
		if (action.is_dynamic) {
			jump = action.fu.df(s, frame_offset + action.sp.get_spu().offset);
		}
		else {
			jump = action.fu.cf(
				s.stack,
				action.sp.is_real_ptr() ?
				action.sp.get_spu() :
				semi_pointer::data(action.sp.get_spu().offset + frame_offset)
			);
		}
		if (jump >= action.jumps.get_used()) {
			throw exception();
		}
		i += action.jumps[jump];
	}
	s.call_stack.pop_back();
	return i - _steps.get_used();
}