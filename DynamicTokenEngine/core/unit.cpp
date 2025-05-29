#include "unit.hpp"
#include "table.hpp"
#include "exceptions/pointer_exception.hpp"
using namespace dte_utils;
void unit::_release_cstr() {
	_data.c_str.~dynamic_cstring();
}
void unit::_release_void() {
	_data.v_val.~strong_ref();
}
void unit::_release_table() {
	_data.t_val.~strong_ref();
}
void unit::_release_unit() {
	_data.u_val.~strong_ref();
}

void unit::_release_type() {
	//there are non-trivial destructors
	switch (_type) {
		case VOID:
			_release_void();
			break;
		case CSTR:
			_release_cstr();
			break;
		case TABLE:
			_release_table();
			break;
		case UNIT:
			_release_unit();
			break;
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

unit::unit(const unit& other) : _type(other.get_type()) {
	switch (other._type) {
		case NIL:
			break;
		case INT:
			new (&_data.i_val) static_array<ptrdiff_t, 3>(other._data.i_val);
			break;
		case FP:
			new (&_data.f_val) static_array<floatpoint, 3>(other._data.f_val);
			break;
		case CSTR:
			new (&_data.c_str) dynamic_cstring(other._data.c_str);
			break;
		case VOID:
			new (&_data.v_val) strong_ref<mem_handler>(other._data.v_val);
			break;
		case TABLE:
			new (&_data.t_val) strong_ref<table>(other._data.t_val);
			break;
		case UNIT:
			new (&_data.u_val) strong_ref<unit>(other._data.u_val);
			break;
	}
}
unit::unit(unit&& other) noexcept : _type(other._type) {
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
	if (_type != other._type) {
		_release_type();
	}
	switch (other._type) {
		case NIL:
			break;
		case INT:
			new (&_data.i_val) static_array<ptrdiff_t, 3>(other._data.i_val);
			break;
		case FP:
			new (&_data.f_val) static_array<floatpoint, 3>(other._data.f_val);
			break;
		case CSTR:
			new (&_data.c_str) dynamic_cstring(other._data.c_str);
			break;
		case VOID:
			new (&_data.v_val) strong_ref<mem_handler>(other._data.v_val);
			break;
		case TABLE:
			new (&_data.t_val) strong_ref<table>(other._data.t_val);
			break;
		case UNIT:
			new (&_data.u_val) strong_ref<unit>(other._data.u_val);
			break;
	}
	_type = other._type;
	return *this;
}
unit& unit::operator=(unit&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	swap_memory(&_data, &other._data, sizeof(data));
	std::swap(_type, other._type);
	return *this;
}


unit& unit::operator=(const static_array<ptrdiff_t, 3>& i_val) {
	if (_type == INT) {
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
	if (_type == FP) {
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
	if (_type == CSTR) {
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
	if (_type == VOID) {
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
	if (_type == TABLE) {
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
	if (_type == UNIT) {
		_data.u_val = u_val;
	}
	else {
		_release_type();
		_type = UNIT;
		new (&_data.u_val) strong_ref<unit>(u_val);
	}
	return *this;
}

size_t unit::get_type() const {
	return _type & type_mask;
}

static_array<ptrdiff_t, 3>& unit::get_int() {
	if (_type == INT) {
		return _data.i_val;
	}
	throw exception(0, "wrong return type");
}
const static_array<ptrdiff_t, 3>& unit::get_int() const {
	if (_type == INT) {
		return _data.i_val;
	}
	throw exception(0, "wrong return type");
}

static_array<floatpoint, 3>& unit::get_fp() {
	if (_type == FP) {
		return _data.f_val;
	}
	throw exception(0, "wrong return type");
}
const static_array<floatpoint, 3>& unit::get_fp() const {
	if (_type == FP) {
		return _data.f_val;
	}
	throw exception(0, "wrong return type");
}

dynamic_cstring& unit::get_cstr() {
	if (_type == CSTR) {
		return _data.c_str;
	}
	throw exception(0, "wrong return type");
}
const dynamic_cstring& unit::get_cstr() const {
	if (_type == CSTR) {
		return _data.c_str;
	}
	throw exception(0, "wrong return type");
}

strong_ref<mem_handler>& unit::get_void() {
	if (_type == VOID) {
		return _data.v_val;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<mem_handler>& unit::get_void() const {
	if (_type == VOID) {
		return _data.v_val;
	}
	throw exception(0, "wrong return type");
}

strong_ref<table>& unit::get_table() {
	if (_type == TABLE) {
		return _data.t_val;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<table>& unit::get_table() const {
	if (_type == TABLE) {
		return _data.t_val;
	}
	throw exception(0, "wrong return type");
}

strong_ref<unit>& unit::get_unit() {
	if (_type == UNIT) {
		return _data.u_val;
	}
	throw exception(0, "wrong return type");
}
const strong_ref<unit>& unit::get_unit() const {
	if (_type == UNIT) {
		return _data.u_val;
	}
	throw exception(0, "wrong return type");
}


void unit::clr_value() {
	_release_type();
	_type = NIL;
}