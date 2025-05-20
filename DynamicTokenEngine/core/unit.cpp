#include "unit.hpp"

#include <iostream>
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
	new (&_data.t_val) mem_wrapper<weak_ref<table>>(cnew<table>());
	++const_cast<ref_counter*>(_data.t_val.get().get_counter())->strong_owners;
}
void unit::_set_table(table&& t_val) {
	new (&_data.t_val) mem_wrapper<weak_ref<table>>(cnew<table>(std::move(t_val)));
	++const_cast<ref_counter*>(_data.t_val.get().get_counter())->strong_owners;
}
void unit::_set_table(const weak_ref<table>& t_pointer) {
	new (&_data.t_val) mem_wrapper<weak_ref<table>>(t_pointer);
	++const_cast<ref_counter*>(_data.t_val.get().get_counter())->strong_owners;
}



void unit::_release_void() {
	_data.mem_val.~mem_handler();
}
void unit::_release_cstr() {
	_data.cstr.~mem_wrapper();
}
void unit::_release_table() {
	const weak_ref<table>& ref = get_table_ref();
	ref_counter* counter = const_cast<ref_counter*>(ref.get_counter());
	if (!--counter->strong_owners) {
		destuct_at(ref.get());
	}
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
			_set_table();
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
	if (t_pointer.expired()) {
		throw bad_weak_ptr();
	}
	if (_type == TABLE) {
		/*weak_ref<table>& ref = get_table_ref();
		ref_counter* counter = const_cast<ref_counter*>(ref.get_counter());
		if (!--counter->strong_owners) {
			ref.get()->~table();
		}
		ref = t_pointer;
		counter = const_cast<ref_counter*>(ref.get_counter());
		++counter->strong_owners;*/


		weak_ref<table>& ref = get_table_ref();
		ref_counter* counter = const_cast<ref_counter*>(ref.get_counter());
		if (!--counter->strong_owners) {
			destuct_at(ref.get());
			std::cout << "D" << std::endl;
		}

		_type = TABLE;
		ref = t_pointer;
		++const_cast<ref_counter*>(
			ref.get_counter()
		)->strong_owners;
		ref;
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

weak_ref<table>& unit::get_table_ref() {
	if (_type == TABLE) {
		return _data.t_val.get();
	}
	throw exception(0, "wrong return type");
}
const weak_ref<table>& unit::get_table_ref() const {
	if (_type == TABLE) {
		return _data.t_val.get();
	}
	throw exception(0, "wrong return type");
}