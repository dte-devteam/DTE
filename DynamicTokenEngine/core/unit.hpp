#pragma once
#include "memory/dynamic_string.hpp"
#include "memory/static_array.hpp"
#include "pointer/strong_ref.hpp"
#include "types.hpp"



struct c_function;
struct dte_function;
struct table;
struct unit {
	enum type : size_t {
		NIL,
		INT,
		FP,
		CSTR,
		VOID,
		TABLE,
		UNIT,
		DFUNC,
		CFUNC
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
		dte_utils::strong_ref<dte_function>				dfunc;	//dte_function
		dte_utils::weak_ref<c_function>					cfunc;	//c_function

	};
	typedef void(*creator)(data&, const data&);
	typedef void(*assigner)(data&, const data&);
	typedef void(*releaser)(data&);
	protected:
		size_t _type;
		data _data;

		static void _create_int(data& dest, const data& src);
		static void _create_fp(data& dest, const data& src);
		static void _create_cstr(data& dest, const data& src);
		static void _create_void(data& dest, const data& src);
		static void _create_table(data& dest, const data& src);
		static void _create_unit(data& dest, const data& src);
		static void _create_dfunc(data& dest, const data& src);
		static void _create_cfunc(data& dest, const data& src);
		static inline const creator _creators[] = {
			nullptr,		//NIL	(no data is constructed)
			_create_int,	//INT
			_create_fp,		//FP
			_create_cstr,	//CSTR
			_create_void,	//VOID
			_create_table,	//TABLE
			_create_unit,	//UNIT
			_create_dfunc,	//DFUNC
			_create_cfunc	//CFUNC
		};


		static void _assign_int(data& dest, const data& src);
		static void _assign_fp(data& dest, const data& src);
		static void _assign_cstr(data& dest, const data& src);
		static void _assign_void(data& dest, const data& src);
		static void _assign_table(data& dest, const data& src);
		static void _assign_unit(data& dest, const data& src);
		static void _assign_dfunc(data& dest, const data& src);
		static void _assign_cfunc(data& dest, const data& src);
		static inline const assigner _assigners[] = {
			nullptr,		//NIL	(no data is assigned)
			_assign_int,	//INT
			_assign_fp,		//FP
			_assign_cstr,	//CSTR
			_assign_void,	//VOID
			_assign_table,	//TABLE
			_assign_unit,	//UNIT
			_assign_dfunc,	//DFUNC
			_assign_cfunc	//CFUNC
		};

		static void _release_cstr(data& dest);
		static void _release_void(data& dest);
		static void _release_table(data& dest);
		static void _release_unit(data& dest);
		static void _release_dfunc(data& dest);
		static void _release_cfunc(data& dest);
		static inline const releaser _releasers[] = {
			nullptr,		//NIL	(no data was constructed)
			nullptr,		//INT	(trivial: ptrdiff_t[3])
			nullptr,		//FP	(trivial: floatpoint[3])
			_release_cstr,	//CSTR
			_release_void,	//VOID
			_release_table,	//TABLE
			_release_unit,	//UNIT
			_release_dfunc,	//DFUNC
			_release_cfunc	//CFUNC
		};

		//should be protected?
		static constexpr size_t type_mask = 0xFFFF;

		void _release_type();
	public:
		unit();
		unit(const dte_utils::static_array<ptrdiff_t, 3>& i_val);
		unit(const dte_utils::static_array<floatpoint, 3>& f_val);
		unit(const dte_utils::dynamic_cstring& c_str);
		unit(const dte_utils::strong_ref<dte_utils::mem_handler>& v_val);
		unit(const dte_utils::strong_ref<table>& t_val);
		unit(const dte_utils::strong_ref<unit>& u_val);
		unit(const dte_utils::strong_ref<dte_function>& dfunc);
		unit(const dte_utils::weak_ref<c_function>& cfunc);

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
		unit& operator=(const dte_utils::strong_ref<dte_function>& dfunc);
		unit& operator=(const dte_utils::weak_ref<c_function>& cfunc);

		size_t get_type() const;
		size_t get_type_with_attr() const;
		void set_attr(size_t attr);
		void clr_attr();

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

		dte_utils::strong_ref<dte_function>& get_dfunc();
		const dte_utils::strong_ref<dte_function>& get_dfunc() const;

		dte_utils::weak_ref<c_function>& get_cfunc();
		const dte_utils::weak_ref<c_function>& get_cfunc() const;


		void clr_value();
};