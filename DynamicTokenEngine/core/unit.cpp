#include "unit.hpp"
#include "table.hpp"
#include "exceptions/pointer_exception.hpp"
using namespace dte_utils;
void unit::_set_void() {
	new (&_data.mem_val) mem_handler();
}
void unit::_set_void(mem_handler&& mem_val) {
	new (&_data.mem_val) mem_handler(std::move(mem_val));
}

void unit::_set_cstr() {
	new (&_data.cstr) mem_wrapper<dynamic_cstring>();
}
void unit::_set_cstr(const dynamic_cstring& cstr) {
	new (&_data.cstr) mem_wrapper<dynamic_cstring>(cstr);
}

void unit::_set_table() {
	place_at(&_data.t_val, cnew<table>());
}
void unit::_set_table(table&& t_val) {
	place_at(&_data.t_val, cnew<table>(std::move(t_val)));
}
void unit::_set_table(const weak_ref<table>& t_pointer) {
	place_at(&_data.t_val, t_pointer);
}



void unit::_release_void() {
	_data.mem_val.~mem_handler();
}
void unit::_release_cstr() {
	_data.cstr.~mem_wrapper();
}
void unit::_release_table() {
	_data.t_val.~mem_wrapper();
}


void unit::_release_type() {
	switch (_type) {
		case unit::VOID:
			_release_void();
			break;
		case unit::CSTR:
			_release_cstr();
			break;
		case unit::TABLE:
			_release_table();
			break;
	}
}


void unit::clr_value() {
	_release_type();
	_type = NIL;
}


unit::unit() : _type(NIL) {}
unit::unit(const ptrdiff_t& i_val) : _type(INT) {
	_data.int_val = i_val;
}
unit::unit(const floatpoint& fp_val) : _type(FP) {
	_data.float_val = fp_val;
}
unit::unit(const func& f_val) : _type(FUNC) {
	_data.func_val = f_val;
}
unit::unit(mem_handler&& m_val) : _type(VOID) {
	_data.int_val = 0; //create nullptr
	_data.mem_val = std::move(m_val);
}
unit::unit(const dynamic_cstring& cstr) : _type(CSTR) {
	new (&_data.mem_val) mem_handler(sizeof(dynamic_cstring));
	new (static_cast<void*>(_data.mem_val)) dynamic_cstring(cstr);
}

unit::unit(const unit& other) : _type(other._type) {
	if (other._type == VOID) {
		throw exception(0, "void can`t be copied!");
	}
	//we can use copy memory by type
	_data.int_val = other._data.int_val;
}
unit::unit(unit&& other) noexcept : _type(other._type) {
	other._type = NIL;
	//we can use copy memory by type
	_data.int_val = other._data.int_val;
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
		case VOID:
			throw exception(0, "void can`t be copied!");
		case CSTR:
			if (_type != CSTR) {
				_set_cstr(other.get_cstr());
			}
			else {
				get_cstr() = other.get_cstr();
			}
			break;
		case TABLE:
			_set_table(other.get_table_ref());
			break;
		default:
			//we can use copy memory by type
			_data.int_val = other._data.int_val;
			break;
	}
	_type = other._type;
	return *this;
}
unit& unit::operator=(unit&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	//we can use copy memory by type
	std::swap(_data.int_val, other._data.int_val);
	std::swap(_type, other._type);
	return *this;
}


unit& unit::operator=(const ptrdiff_t& int_val) {
	_release_type();
	_type = INT;
	_data.int_val = int_val;
	return *this;
}
unit& unit::operator=(const floatpoint& float_val) {
	_release_type();
	_type = FP;
	_data.float_val = float_val;
	return *this;
}
unit& unit::operator=(const func& func_val) {
	_release_type();
	_type = FUNC;
	_data.func_val = func_val;
	return *this;
}


unit& unit::operator=(mem_handler&& mem_val) {
	if (_type == VOID) {
		get_void() = std::move(mem_val);
	}
	else {
		_release_type();
		_type = VOID;
		_set_void(std::move(mem_val));
	}
	return *this;
}
unit& unit::operator=(const dynamic_cstring& cstr) {
	if (_type == CSTR) {
		get_cstr() = cstr;
	}
	else {
		_release_type();
		_type = CSTR;
		_set_cstr(cstr);
	}
	return *this;
}
unit& unit::operator=(table&& t_val) {
	if (_type == TABLE) {
		get_table() = std::move(t_val);
	}
	else {
		_release_type();
		_type = TABLE;
		_set_table(std::move(t_val));
	}
	return *this;
}
unit& unit::operator=(const weak_ref<table>& t_pointer) {
	if (_type == TABLE) {
		get_table_ref() = t_pointer;
	}
	else {
		_release_type();
		_type = TABLE;
		_set_table(t_pointer);
	}
	return *this;
}


ptrdiff_t& unit::get_int() {
	if (_type == INT) {
		return _data.int_val;
	}
	throw exception(0, "wrong return type");
}
const ptrdiff_t& unit::get_int() const {
	if (_type == INT) {
		return _data.int_val;
	}
	throw exception(0, "wrong return type");
}

floatpoint& unit::get_fp() {
	if (_type == FP) {
		return _data.float_val;
	}
	throw exception(0, "wrong return type");
}
const floatpoint& unit::get_fp() const {
	if (_type == FP) {
		return _data.float_val;
	}
	throw exception(0, "wrong return type");
}


func& unit::get_func() {
	if (_type == FUNC) {
		return _data.func_val;
	}
	throw exception(0, "wrong return type");
}
const func& unit::get_func() const {
	if (_type == FUNC) {
		return _data.func_val;
	}
	throw exception(0, "wrong return type");
}


mem_handler& unit::get_void() {
	if (_type == VOID) {
		return _data.mem_val;
	}
	throw exception(0, "wrong return type");
}
const mem_handler& unit::get_void() const {
	if (_type == VOID) {
		return _data.mem_val;
	}
	throw exception(0, "wrong return type");
}

dynamic_cstring& unit::get_cstr() {
	if (_type == CSTR) {
		return _data.cstr.get();
	}
	throw exception(0, "wrong return type");
}
const dynamic_cstring& unit::get_cstr() const {
	if (_type == CSTR) {
		return _data.cstr.get();
	}
	throw exception(0, "wrong return type");
}

table& unit::get_table() {
	if (_type == TABLE) {
		return *get_table_ref().get();
	}
	throw exception(0, "wrong return type");
}
const table& unit::get_table() const {
	if (_type == TABLE) {
		return *get_table_ref().get();
	}
	throw exception(0, "wrong return type");
}

strong_ref<table>& unit::get_table_ref() {
	if (_type == TABLE) {
		return _data.t_val.get();
	}
	throw exception(0, "wrong return type");
}
const strong_ref<table>& unit::get_table_ref() const {
	if (_type == TABLE) {
		return _data.t_val.get();
	}
	throw exception(0, "wrong return type");
}