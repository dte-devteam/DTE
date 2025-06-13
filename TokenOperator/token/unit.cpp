#include "unit.hpp"
#include "exceptions/pointer_exception.hpp"
//members
#include "table.hpp"
#include "dte_function.hpp"
using namespace dte_utils;
using namespace dte_token;
void unit::_create_int(data& dest, const data& src) {
	new (&dest.i_val) static_array<ptrdiff_t, 3>(src.i_val);
}
void unit::_create_fp(data& dest, const data& src) {
	new (&dest.f_val) static_array<floatpoint, 3>(src.f_val);
}
void unit::_create_cstr(data& dest, const data& src) {
	new (&dest.c_str) dynamic_cstring(src.c_str);
}
void unit::_create_void(data& dest, const data& src) {
	new (&dest.v_val) strong_ref<mem_handler>(src.v_val);
}
void unit::_create_table(data& dest, const data& src) {
	new (&dest.t_val) strong_ref<table>(src.t_val);
}
void unit::_create_unit(data& dest, const data& src) {
	new (&dest.u_val) strong_ref<unit>(src.u_val);
}
void unit::_create_dfunc(data& dest, const data& src) {
	new (&dest.dfunc) strong_ref<dte_function>(src.dfunc);
}
void unit::_create_cfunc(data& dest, const data& src) {
	new (&dest.cfunc) weak_ref<c_function>(src.cfunc);
}


void unit::_assign_int(data& dest, const data& src) {
	dest.i_val = src.i_val;
}
void unit::_assign_fp(data& dest, const data& src) {
	dest.f_val = src.f_val;
}
void unit::_assign_cstr(data& dest, const data& src) {
	dest.c_str = src.c_str;
}
void unit::_assign_void(data& dest, const data& src) {
	dest.v_val = src.v_val;
}
void unit::_assign_table(data& dest, const data& src) {
	dest.t_val = src.t_val;
}
void unit::_assign_unit(data& dest, const data& src) {
	dest.u_val = src.u_val;
}
void unit::_assign_dfunc(data& dest, const data& src) {
	dest.dfunc = src.dfunc;
}
void unit::_assign_cfunc(data& dest, const data& src) {
	dest.cfunc = src.cfunc;
}


void unit::_release_cstr(data& dest) {
	dest.c_str.~dynamic_cstring();
}
void unit::_release_void(data& dest) {
	dest.v_val.~strong_ref();
}
void unit::_release_table(data& dest) {
	dest.t_val.~strong_ref();
}
void unit::_release_unit(data& dest) {
	dest.u_val.~strong_ref();
}
void unit::_release_dfunc(data& dest) {
	dest.dfunc.~strong_ref();
}
void unit::_release_cfunc(data& dest) {
	dest.cfunc.~weak_ref();
}

void unit::_release_type() {
	if (_releasers[get_type()]) {
		_releasers[get_type()](_data);
	}
}




unit::unit() : _type(NIL) {}
unit::unit(const static_array<ptrdiff_t, 3>& i_val) : _type(INT) {
	new (&_data.i_val) static_array<ptrdiff_t, 3>(i_val);
}
unit::unit(const static_array<floatpoint, 3>& f_val) : _type(FP) {
	new (&_data.f_val) static_array<floatpoint, 3>(f_val);
}
unit::unit(const dynamic_cstring& c_str) : _type(CSTR) {
	new (&_data.c_str) dynamic_cstring(c_str);
}
unit::unit(const strong_ref<mem_handler>& v_val) : _type(VOID) {
	new (&_data.v_val) strong_ref<mem_handler>(v_val);
}
unit::unit(const strong_ref<table>& t_val) : _type(TABLE) {
	new (&_data.t_val) strong_ref<table>(t_val);
}
unit::unit(const strong_ref<unit>& u_val) : _type(TABLE) {
	new (&_data.u_val) strong_ref<unit>(u_val);
}
unit::unit(const dte_utils::strong_ref<dte_function>& dfunc) : _type(DFUNC) {
	new (&_data.dfunc) strong_ref<dte_function>(dfunc);
}
unit::unit(const dte_utils::weak_ref<c_function>& cfunc) : _type(CFUNC) {
	new (&_data.cfunc) weak_ref<c_function>(cfunc);
}

unit::unit(const unit& other) : _type(other.get_type()) {
	if (_creators[other.get_type()]) {
		_creators[other.get_type()](_data, other._data);
	}
}
unit::unit(unit&& other) noexcept : _type(other.get_type()) {
	other._type = NIL;
	copy_memory(&_data.i_val, &other._data.i_val, sizeof(data));
}

unit::~unit() {
	_release_type();
}


unit& unit::operator=(const unit& other) {
	if (this == &other) {
		return *this;
	}
	if (get_type() != other.get_type()) {
		_release_type();
		_type = other.get_type();
		if (_creators[get_type()]) {
			_creators[get_type()](_data, other._data);
		}
	}
	else if (_assigners[get_type()]) {
		_assigners[get_type()](_data, other._data);
	}
	return *this;
}
unit& unit::operator=(unit&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	swap_memory(&_data, &other._data, sizeof(data));
	std::swap(_type, other._type);
	//TODO: type can have qualifiers
	return *this;
}


unit& unit::operator=(const static_array<ptrdiff_t, 3>& i_val) {
	if (get_type() == INT) {
		_data.i_val = i_val;
	}
	else {
		_release_type();
		_type = INT;
		new (&_data.i_val) static_array<ptrdiff_t, 3>(i_val);
	}
	return *this;
}
unit& unit::operator=(const static_array<floatpoint, 3>& f_val) {
	if (get_type() == FP) {
		_data.f_val = f_val;
	}
	else {
		_release_type();
		_type = FP;
		new (&_data.f_val) static_array<floatpoint, 3>(f_val);
	}
	return *this;
}
unit& unit::operator=(const dynamic_cstring& c_str) {
	if (get_type() == CSTR) {
		_data.c_str = c_str;
	}
	else {
		_release_type();
		_type = CSTR;
		new (&_data.c_str) dynamic_cstring(c_str);
	}
	return *this;
}
unit& unit::operator=(const strong_ref<mem_handler>& v_val) {
	if (get_type() == VOID) {
		_data.v_val = v_val;
	}
	else {
		_release_type();
		_type = VOID;
		new (&_data.v_val) strong_ref<mem_handler>(v_val);
	}
	return *this;
}
unit& unit::operator=(const strong_ref<table>& t_val) {
	if (get_type() == TABLE) {
		_data.t_val = t_val;
	}
	else {
		_release_type();
		_type = TABLE;
		new (&_data.t_val) strong_ref<table>(t_val);
	}
	return *this;
}
unit& unit::operator=(const strong_ref<unit>& u_val) {
	if (get_type() == UNIT) {
		_data.u_val = u_val;
	}
	else {
		_release_type();
		_type = UNIT;
		new (&_data.u_val) strong_ref<unit>(u_val);
	}
	return *this;
}
unit& unit::operator=(const dte_utils::strong_ref<dte_function>& dfunc) {
	if (get_type() == DFUNC) {
		_data.dfunc = dfunc;
	}
	else {
		_release_type();
		_type = DFUNC;
		new (&_data.dfunc) strong_ref<dte_function>(dfunc);
	}
	return *this;
}
unit& unit::operator=(const dte_utils::weak_ref<c_function>& cfunc) {
	if (get_type() == CFUNC) {
		_data.cfunc = cfunc;
	}
	else {
		_release_type();
		_type = CFUNC;
		new (&_data.cfunc) weak_ref<c_function>(cfunc);
	}
	return *this;
}


size_t unit::get_type() const {
	return get_type_with_attr() & 0xFFFF;
}
size_t unit::get_type_with_attr() const {
	return _type;
}
void unit::set_attr(size_t attr) {
	_type = get_type() | (attr & ~0xFFFF);
}
void unit::clr_attr() {
	_type = get_type();
}

static_array<ptrdiff_t, 3>& unit::get_int() {
	if (get_type() == INT) {
		return _data.i_val;
	}
	throw exception(0, "wrong return type");
}
const static_array<ptrdiff_t, 3>& unit::get_int() const {
	if (get_type() == INT) {
		return _data.i_val;
	}
	throw exception(0, "wrong return type");
}

static_array<floatpoint, 3>& unit::get_fp() {
	if (get_type() == FP) {
		return _data.f_val;
	}
	throw exception(0, "wrong return type");
}
const static_array<floatpoint, 3>& unit::get_fp() const {
	if (get_type() == FP) {
		return _data.f_val;
	}
	throw exception(0, "wrong return type");
}

dynamic_cstring& unit::get_cstr() {
	if (get_type() == CSTR) {
		return _data.c_str;
	}
	throw exception(0, "wrong return type");
}
const dynamic_cstring& unit::get_cstr() const {
	if (get_type() == CSTR) {
		return _data.c_str;
	}
	throw exception(0, "wrong return type");
}

strong_ref<mem_handler>& unit::get_void() {
	if (get_type() == VOID) {
		return _data.v_val;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<mem_handler>& unit::get_void() const {
	if (get_type() == VOID) {
		return _data.v_val;
	}
	throw exception(0, "wrong return type");
}

strong_ref<table>& unit::get_table() {
	if (get_type() == TABLE) {
		return _data.t_val;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<table>& unit::get_table() const {
	if (get_type() == TABLE) {
		return _data.t_val;
	}
	throw exception(0, "wrong return type");
}

strong_ref<unit>& unit::get_unit() {
	if (get_type() == UNIT) {
		return _data.u_val;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<unit>& unit::get_unit() const {
	if (get_type() == UNIT) {
		return _data.u_val;
	}
	throw exception(0, "wrong return type");
}

strong_ref<dte_function>& unit::get_dfunc() {
	if (get_type() == DFUNC) {
		return _data.dfunc;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<dte_function>& unit::get_dfunc() const {
	if (get_type() == DFUNC) {
		return _data.dfunc;
	}
	throw exception(0, "wrong return type");
}

dte_utils::weak_ref<c_function>& unit::get_cfunc() {
	if (get_type() == CFUNC) {
		return _data.cfunc;
	}
	throw exception(0, "wrong return type");
}
const dte_utils::weak_ref<c_function>& unit::get_cfunc() const {
	if (get_type() == CFUNC) {
		return _data.cfunc;
	}
	throw exception(0, "wrong return type");
}


void unit::clr_value() {
	_release_type();
	_type = NIL;
}