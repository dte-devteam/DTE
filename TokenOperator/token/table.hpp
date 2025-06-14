#pragma once
#include "unit.hpp"
namespace dte_token {
	struct table_unit {
		unit u;
		dte_utils::dynamic_cstring name;
	};
	struct table {
		protected:
			dte_utils::dynamic_array<table_unit> _table_units;
		public:
			table() = default;
			table(const dte_utils::dynamic_array<table_unit>& units);
			table(dte_utils::dynamic_array<table_unit>&& units);
			table(const table& other);
			table(table&& other) noexcept;
			dte_utils::dynamic_array<table_unit>& get_table_units();
			const dte_utils::dynamic_array<table_unit>& get_table_units() const;
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