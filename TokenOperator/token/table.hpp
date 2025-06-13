#pragma once
#include "unit.hpp"
namespace dte_token {
	struct table_unit {
		unit u;
		dte_utils::dynamic_cstring name;
	};
	struct table {
		dte_utils::dynamic_array<table_unit> _t_units;

		table() = default;
		table(const table& other);
		table(table&& other) noexcept;

		table& operator=(const table& other);
		table& operator=(table&& other) noexcept;



		//won`t throw
		table_unit& get_as_target(const dte_utils::dynamic_cstring& name);
		//won`t throw
		//carefull: can return nullptr!
		table_unit* get_as_source(const dte_utils::dynamic_cstring& name);

		//will throw if out of range
		table_unit& get_by_index(size_t index);
	};
}