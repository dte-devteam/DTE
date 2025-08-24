#include "c_func_unit.hpp"
using namespace dte_module;
using namespace dte_token;
using namespace dte_utils;
c_func_unit::c_func_unit(c_function::func* body, const c_function::metadata& meta, destructor* args_destructor) : _c_func(body, meta, args_destructor), _c_func_ptr(&_c_func) {}
c_func_unit::c_func_unit(c_function::func* body, c_function::metadata&& meta, destructor* args_destructor) : _c_func(body, std::move(meta), args_destructor), _c_func_ptr(&_c_func) {}
const atomic_weak_ref<const c_function>& c_func_unit::get_c_func_ptr() const noexcept {
	return _c_func_ptr;
}