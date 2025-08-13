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



using f_step = dte_function::step;
f_step::step(const dte_utils::atomic_strong_ref<c_function>& c_func, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp) :
_is_dynamic(false), _jumps(jumps), _semi_ptr(sp) {
	new (&_func_unit.c_func) atomic_strong_ref<c_function>(c_func);
}
f_step::step(const atomic_strong_ref<dte_function>& dte_func, const dynamic_array<size_t>& jumps, const semi_pointer& sp) :
_is_dynamic(true), _jumps(jumps), _semi_ptr(sp) {
	new (&_func_unit.dte_func) atomic_strong_ref<dte_function>(dte_func);
}
f_step::step(const step& other) : _is_dynamic(other.get_is_dynamic()), _semi_ptr(other.get_semi_ptr()), _jumps(other.get_jumps()) {
	if (get_is_dynamic()) {
		new (&_func_unit.dte_func) atomic_strong_ref<dte_function>(other.get_function_unit().dte_func);
	}
	else {
		new (&_func_unit.c_func) atomic_strong_ref<c_function>(other.get_function_unit().c_func);
	}
}
f_step::~step() {
	if (get_is_dynamic()) {
		_func_unit.dte_func.~strong_ref();
	}
	else {
		_func_unit.c_func.~strong_ref();
	}
}
const f_step::function_unit& f_step::get_function_unit() const noexcept {
	return _func_unit;
}
bool f_step::get_is_dynamic() const noexcept {
	return _is_dynamic;
}
const dynamic_array<size_t>& f_step::get_jumps() const noexcept {
	return _jumps;
}
const semi_pointer& f_step::get_semi_ptr() const noexcept {
	return _semi_ptr;
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
		if (action.get_is_dynamic()) {
			jump = action.get_function_unit().dte_func(s, frame_offset + action.get_semi_ptr().get_spu().offset);
		}
		else {
			jump = action.get_function_unit().c_func(
				s.stack,
				action.get_semi_ptr().is_real_ptr() ?
				action.get_semi_ptr().get_spu() :
				semi_pointer::data(action.get_semi_ptr().get_spu().offset + frame_offset)
			);
		}
		if (jump >= action.get_jumps().get_used()) {
			throw exception();
		}
		i += action.get_jumps()[jump];
	}
	s.call_stack.pop_back();
	return i - _steps.get_used();
}