#pragma once
#include "unit.hpp"

#include <iostream>
using namespace dte_utils;
struct table_unit {
	unit u;
	dynamic_cstring name;
};
struct table {
	dynamic_array<table_unit> _t_units;

	table() = default;
	table(const table& other);
	table(table&& other) noexcept;
	~table();

	table& operator=(const table& other);
	table& operator=(table&& other) noexcept;



	//won`t throw
	table_unit& get_as_target(const dynamic_cstring& name);
	//won`t throw
	//carefull: can return nullptr!
	table_unit* get_as_source(const dynamic_cstring& name);

	//will throw if out of range
	table_unit& get_by_index(size_t index);
};