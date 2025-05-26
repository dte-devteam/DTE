#pragma once
#include "memory/dynamic_string.hpp"
#include "memory/mem_wrapper.hpp"
#include "pointer/strong_ref.hpp"
#include "types.hpp"
using namespace dte_utils;
struct function_stack;
using func = size_t(*)(function_stack&, size_t);//TODO

struct table;
struct unit {
	static const size_t char_num = sizeof(ptrdiff_t);
	enum type : size_t {
		NIL,
		INT,
		FP,
		FUNC,
		VOID,
		CSTR,
		TABLE
	};
	//MUST BE MANUALY CONTROLLED
	union data {
		data() {}
		~data() {}
		ptrdiff_t	int_val;				//integer
		floatpoint	float_val;				//float point
		func		func_val;				//function
		mem_handler	mem_val;				//void block
		tmem_wrapper<dynamic_cstring> cstr;	//cstring
		tmem_wrapper<strong_ref<table>> t_val;	//table
	};
	protected:
		type _type;
		data _data;

		void _set_void();
		void _set_void(mem_handler&& mem_val);

		void _set_cstr();
		void _set_cstr(const dynamic_cstring& cstr);
		template<size_t N>
		void _set_cstr(const char(&arr)[N]) {
			new (&_data.cstr) mem_wrapper<dynamic_cstring>(arr);
		}

		void _set_table();
		void _set_table(table&& t_val);
		void _set_table(const weak_ref<table>& t_pointer);

		void _release_void();
		void _release_cstr();
		void _release_table();

		void _release_type();
	public:
		unit();
		unit(const ptrdiff_t& i_val);
		unit(const floatpoint& fp_val);
		unit(const func& f_val);
		unit(mem_handler&& m_val);
		unit(const dynamic_cstring& cstr);

		unit(const unit& other);
		unit(unit&& other) noexcept;
		~unit();

		unit& operator=(const unit& other);
		unit& operator=(unit&& other) noexcept;


		unit& operator=(const ptrdiff_t& int_val);
		unit& operator=(const floatpoint& float_val);
		unit& operator=(const func& func_val);
		unit& operator=(mem_handler&& mem_val);
		unit& operator=(const dynamic_cstring& cstr);
		template<size_t N>
		unit& operator=(const char (&arr)[N]) {
			if (_type == CSTR) {
				get_cstr() = arr;
			}
			else {
				_release_type();
				_type = CSTR;
				_set_cstr(arr);
			}
			return *this;
		}
		unit& operator=(table&& t_val);
		unit& operator=(const weak_ref<table>& t_pointer);



		ptrdiff_t& get_int();
		const ptrdiff_t& get_int() const;

		floatpoint& get_fp();
		const floatpoint& get_fp() const;

		func& get_func();
		const func& get_func() const;

		mem_handler& get_void();
		const mem_handler& get_void() const;

		dynamic_cstring& get_cstr();
		const dynamic_cstring& get_cstr() const;

		table& get_table();
		const table& get_table() const;

		strong_ref<table>& get_table_ref();
		const strong_ref<table>& get_table_ref() const;



		void clr_value();
};