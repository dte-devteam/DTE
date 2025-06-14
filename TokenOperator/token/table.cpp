#include "table.hpp"
using namespace dte_utils;
using namespace dte_token;
table::table(const dynamic_array<table_unit>& units) : _table_units(units) {}
table::table(dynamic_array<table_unit>&& units) : _table_units(std::move(units)) {}
table::table(const table& other) : table(other.get_table_units()) {}
table::table(table&& other) noexcept : table(std::move(other._table_units)) {}
dynamic_array<table_unit>& table::get_table_units() {
	return _table_units;
}
const dynamic_array<table_unit>& table::get_table_units() const {
	return _table_units;
}
table& table::operator=(const table& other) {
	_table_units.clear();
	_table_units = other._table_units;
	return *this;
}
table& table::operator=(table&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(_table_units, other._table_units);
	return *this;
}



table_unit& table::get_as_target(const dynamic_cstring& name) {
	//won`t throw
	for (table_unit& tu : _table_units) {
		if (tu.name == name) {
			return tu;
		}
	}
	_table_units.emplace_back(unit(), name);
	return _table_units.back();
}

table_unit* table::get_as_source(const dynamic_cstring& name) {
	//won`t throw
	//carefull: can return nullptr!
	return _table_units.find([&name](const table_unit& tu) { return tu.name == name; });
}

table_unit& table::get_by_index(size_t index) {
	//will throw if out of range
	return _table_units[index];
}