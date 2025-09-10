#include "c_function.hpp"
#include "data_stack.hpp"
using namespace dte_utils;
using namespace dte_token;
//c_function::c_function(func* body, const metadata& meta, destructor* args_destructor) : _body(body), _meta(meta), _args_destructor(args_destructor) {}
//c_function::c_function(func* body, metadata&& meta, destructor* args_destructor) : _body(body), _meta(std::move(meta)), _args_destructor(args_destructor) {}
/*c_function::c_function(const c_function& other) : _meta(other.get_meta()), _body(other.get_body()), _args_destructor(other.get_args_destructor()) {}
c_function::c_function(c_function&& other) noexcept : _body(other.get_body()), _meta(std::move(other._meta)), _args_destructor(other.get_args_destructor()) {}
const c_function::metadata& c_function::get_meta() const noexcept {
	return _meta;
}
c_function::func* c_function::get_body() const noexcept {
	return _body;
}
destructor* c_function::get_args_destructor() const noexcept {
	return _args_destructor;
}*/
size_t c_function::operator()(data_stack& stack, const semi_pointer::data& spd) const {
	return _body(stack, spd);
}