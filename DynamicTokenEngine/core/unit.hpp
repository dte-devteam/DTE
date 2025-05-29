#pragma once
#include "memory/dynamic_string.hpp"
#include "memory/static_array.hpp"
#include "pointer/strong_ref.hpp"
#include "types.hpp"
struct function_stack;
using func = size_t(*)(function_stack&, size_t);//TODO


struct table;
struct unit {
	static const size_t type_mask = size_t(-1) >> (sizeof(size_t) * 8 - 16);
	enum type : size_t {
		NIL,
		INT,
		FP,
		CSTR,
		VOID,
		TABLE,
		UNIT
	};
	union data {
		data() {}
		~data() {}
		dte_utils::static_array<ptrdiff_t, 3>			i_val;	//integer
		dte_utils::static_array<floatpoint, 3>			f_val;	//float point
		dte_utils::dynamic_cstring						c_str;	//cstring
		dte_utils::strong_ref<dte_utils::mem_handler>	v_val;	//void block
		dte_utils::strong_ref<table>					t_val;	//table
		dte_utils::strong_ref<unit>						u_val;	//unit
	};
	protected:
		size_t _type;
		data _data;

		void _release_cstr();
		void _release_void();
		void _release_table();
		void _release_unit();

		void _release_type();
	public:
		unit();
		unit(const dte_utils::static_array<ptrdiff_t, 3>& i_val);
		unit(const dte_utils::static_array<floatpoint, 3>& f_val);
		unit(const dte_utils::dynamic_cstring& c_str);
		unit(const dte_utils::strong_ref<dte_utils::mem_handler>& v_val);
		unit(const dte_utils::strong_ref<table>& t_val);
		unit(const dte_utils::strong_ref<unit>& u_val);

		unit(const unit& other);
		unit(unit&& other) noexcept;

		~unit();

		unit& operator=(const unit& other);
		unit& operator=(unit&& other) noexcept;

		unit& operator=(const dte_utils::static_array<ptrdiff_t, 3>& i_val);
		unit& operator=(const dte_utils::static_array<floatpoint, 3>& f_val);
		unit& operator=(const dte_utils::dynamic_cstring& c_str);
		template<size_t N>
		unit& operator=(const char(&arr)[N]) {
			if (_type == CSTR) {
				_data.c_str = arr;
			}
			else {
				_release_type();
				_type = CSTR;
				new (&_data.c_str) dte_utils::dynamic_cstring(arr);
			}
			return *this;
		}
		unit& operator=(const dte_utils::strong_ref<dte_utils::mem_handler>& v_val);
		unit& operator=(const dte_utils::strong_ref<table>& t_val);
		unit& operator=(const dte_utils::strong_ref<unit>& u_val);

		size_t get_type() const;

		dte_utils::static_array<ptrdiff_t, 3>& get_int();
		const dte_utils::static_array<ptrdiff_t, 3>& get_int() const;

		dte_utils::static_array<floatpoint, 3>& get_fp();
		const dte_utils::static_array<floatpoint, 3>& get_fp() const;

		dte_utils::dynamic_cstring& get_cstr();
		const dte_utils::dynamic_cstring& get_cstr() const;

		dte_utils::strong_ref<dte_utils::mem_handler>& get_void();
		const dte_utils::strong_ref<dte_utils::mem_handler>& get_void() const;

		dte_utils::strong_ref<table>& get_table();
		const dte_utils::strong_ref<table>& get_table() const;

		dte_utils::strong_ref<unit>& get_unit();
		const dte_utils::strong_ref<unit>& get_unit() const;


		void clr_value();
};