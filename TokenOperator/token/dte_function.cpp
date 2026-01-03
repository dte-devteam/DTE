
#include "dte_function.hpp"
#include "stream.hpp"
#include "c_function.hpp"
#include "exceptions/error_codes.hpp"
#include "exceptions/error_desc.hpp"
using namespace dte_utils;
using namespace dte_token;

//dte_function::dte_function(metadata meta, dynamic_array<step> steps) : _meta(meta),
//dte_function::dte_function(const dte_function& other);
//dte_function::dte_function(dte_function&& other);



using f_step = dte_function::step;
using f_unit = f_step::function_unit;

f_unit::function_unit(const atomic_weak_ref<const c_function>& c_func) : c_func(c_func) {}
f_unit::function_unit(const atomic_weak_ref<dte_function>& dte_func) : weak_dte_func(dte_func) {}
f_unit::function_unit(const atomic_strong_ref<dte_function>& dte_func) : strong_dte_func(dte_func) {}

f_step::step(const atomic_weak_ref<const c_function>& c_func, const dynamic_array<size_t>& jumps, const semi_pointer& sp) :
_is_dynamic(false), _is_weak(true), _jumps(jumps), _semi_ptr(sp), _func_unit(c_func) {}
f_step::step(const atomic_weak_ref<const c_function>& c_func, dynamic_array<size_t>&& jumps, const semi_pointer& sp) :
_is_dynamic(false), _is_weak(true), _jumps(std::move(jumps)), _semi_ptr(sp), _func_unit(c_func) {}
f_step::step(const dte_utils::atomic_weak_ref<dte_function>& dte_func, const dte_utils::dynamic_array<size_t>& jumps, const semi_pointer& sp) :
_is_dynamic(true), _is_weak(true), _jumps(jumps), _semi_ptr(sp), _func_unit(dte_func) {}
f_step::step(const dte_utils::atomic_weak_ref<dte_function>& dte_func, dte_utils::dynamic_array<size_t>&& jumps, const semi_pointer& sp) : 
_is_dynamic(true), _is_weak(true), _jumps(std::move(jumps)), _semi_ptr(sp), _func_unit(dte_func) {}
f_step::step(const atomic_strong_ref<dte_function>& dte_func, const dynamic_array<size_t>& jumps, const semi_pointer& sp) :
_is_dynamic(true), _is_weak(false), _jumps(jumps), _semi_ptr(sp), _func_unit(dte_func) {}
f_step::step(const atomic_strong_ref<dte_function>& dte_func, dynamic_array<size_t>&& jumps, const semi_pointer& sp) :
_is_dynamic(true), _is_weak(false), _jumps(std::move(jumps)), _semi_ptr(sp), _func_unit(dte_func) {}
f_step::step(const step& other) : _is_dynamic(other.get_is_dynamic()), _is_weak(other.get_is_weak()), _semi_ptr(other.get_semi_ptr()), _jumps(other.get_jumps()) {
	if (get_is_dynamic()) {
		if (get_is_weak()) {
			place_at(pointer_base<atomic_weak_ref<dte_function>>(&_func_unit.weak_dte_func), other._func_unit.weak_dte_func);
		}
		else {
			place_at(pointer_base<atomic_strong_ref<dte_function>>(&_func_unit.strong_dte_func), other._func_unit.strong_dte_func);
		}
	}
	else {
		place_at(pointer_base<atomic_weak_ref<const c_function>>(&_func_unit.c_func), other._func_unit.c_func);
	}
}
f_step::step(const step&& other) noexcept : _is_dynamic(other.get_is_dynamic()), _is_weak(other.get_is_weak()), _semi_ptr(other.get_semi_ptr()), _jumps(std::move(other.get_jumps())) {
	if (get_is_dynamic()) {
		if (get_is_weak()) {
			place_at(pointer_base<atomic_weak_ref<dte_function>>(&_func_unit.weak_dte_func), other._func_unit.weak_dte_func);
		}
		else {
			place_at(pointer_base<atomic_strong_ref<dte_function>>(&_func_unit.strong_dte_func), other._func_unit.strong_dte_func);
		}
	}
	else {
		place_at(pointer_base<atomic_weak_ref<const c_function>>(&_func_unit.c_func), other._func_unit.c_func);
	}
}
f_step::~step() {
	if (get_is_dynamic()) {
		if (get_is_weak()) {
			_func_unit.weak_dte_func.~weak_ref();
		}
		else {
			_func_unit.strong_dte_func.~strong_ref();
		}
	}
	else {
		_func_unit.c_func.~weak_ref();
	}
}
bool f_step::get_is_dynamic() const noexcept {
	return _is_dynamic;
}
bool f_step::get_is_weak() const noexcept{
	return _is_weak;
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
	steps = std::move(other.steps);
	meta = std::move(other.meta);
	return *this;
}
size_t dte_function::operator()(stream& s, size_t frame_offset, size_t index) const {
	s.call_stack.push_back(this);
	while (index < steps.get_used()) {
		s.functional_index = index;
		const step& action = steps[index];
		size_t jump;
		if (action.get_is_dynamic()) {
			jump = action.get_dte_function<true>()(s, frame_offset + action.get_semi_ptr().get_spu().offset);
		}
		else {
			jump = action.get_c_function<true>()(
				s.stack,
				action.get_semi_ptr().is_real_ptr() ?
				action.get_semi_ptr().get_spu() :
				semi_pointer::data(action.get_semi_ptr().get_spu().offset + frame_offset)
			);
		}
		if (jump >= action.get_jumps().get_used()) {
			throw exception();
		}
		index += action.get_jumps().operator[]<true>(jump);
	}
	s.call_stack.pop_back();
	return index - steps.get_used();
}
dte_function::~dte_function() {
	for (const step& s : steps) {
		if (!s.get_is_dynamic()) {
			if (s.get_c_function().get_args_destructor() && s.get_semi_ptr().is_real_ptr()) {
				s.get_c_function().get_args_destructor()(s.get_semi_ptr().get_spu().ptr);
				aligned_free(s.get_semi_ptr().get_spu().ptr);
			}
		}
	}
}