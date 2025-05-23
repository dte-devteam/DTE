#pragma once
#include "unit.hpp"

#include <iostream>
using namespace dte_utils;
struct table_unit {
	unit u;
	dynamic_cstring name;
};
struct table {
	size_t _index_offset;
	dynamic_array<table_unit> _t_units;

	table() : _index_offset(0) {}
	table(const table&) = delete;	//VOID isn`t copible
	table(table&& other) noexcept : _index_offset(other._index_offset), _t_units(std::move(other._t_units)) {
		other._index_offset = 0;
	}


	table& operator=(const table&) = delete;	//VOID isn`t copible
	table& operator=(table&& other) noexcept {
		if (this == &other) {
			return *this;
		}
		std::swap(_index_offset, other._index_offset);
		std::swap(_t_units, other._t_units);
		return *this;
	}

	table_unit& operator[](size_t index) {
		return _t_units[index + _index_offset];
	}
	table_unit& operator[](const dynamic_cstring& name) {
		for (table_unit& tu : _t_units) {
			if (tu.name == name) {
				return tu;
			}
		}
		throw exception();
	}
	~table() {
		std::cout << "RELEASED" << std::endl;
	}
};