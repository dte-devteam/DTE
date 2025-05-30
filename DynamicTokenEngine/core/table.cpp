#include "table.hpp"
using namespace dte_utils;
table::table(const table& other) : _t_units(other._t_units) {}
table::table(table&& other) noexcept : _t_units(std::move(other._t_units)) {}
table::~table() {
	std::cout << "RELEASED" << std::endl;
}

table& table::operator=(const table& other) {
	_t_units.clear();
	_t_units = other._t_units;
	return *this;
}
table& table::operator=(table&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(_t_units, other._t_units);
	return *this;
}



table_unit& table::get_as_target(const dynamic_cstring& name) {
	//won`t throw
	for (table_unit& tu : _t_units) {
		if (tu.name == name) {
			return tu;
		}
	}
	_t_units.emplace_back(unit(), name);
	return _t_units.back();
}


table_unit* table::get_as_source(const dynamic_cstring& name) {
	//won`t throw
	//carefull: can return nullptr!
	return _t_units.find([&name](const table_unit& tu) { return tu.name == name; });
}

table_unit& table::get_by_index(size_t index) {
	//will throw if out of range
	return _t_units[index];
}