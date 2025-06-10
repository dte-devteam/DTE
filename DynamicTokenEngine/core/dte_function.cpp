#include "dte_function.hpp"
#include "stream.hpp"
#include "c_function.hpp"
using namespace dte_utils;
size_t dte_function::operator()(stream& s, size_t frame_offset) {
	s.call_stack.push_back(this);
	size_t i = 0;
	while (i < _steps.get_used()) {
		const step& action = _steps[i];
		if (action.is_executable) {
			frame_offset += action.delta_frame;
			if (action.data.get_type() == unit::DFUNC) {
				i += action.data.get_dfunc()(s, frame_offset);
			}
			else if (action.data.get_cfunc().expired()) {
				throw exception(0, "function is unloaded");
			}
			else {
				i += action.data.get_cfunc()(s.stack, frame_offset);
			}
		}
		else {
			s.stack.pop(action.delta_frame);
			new (s.stack.push_real(sizeof(unit))) unit(action.data);
		}
		i += action.delta_jump;
	}
	s.call_stack.pop_back();
	return i - _steps.get_used();
}