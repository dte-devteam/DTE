#pragma once
#include "memory/dynamic_string.hpp"
#include "memory/static_array.hpp"
#include "memory/mem_wrapper.hpp"
#include "pointer/strong_ref.hpp"
#include "types.hpp"
using namespace dte_utils;
/*
struct type {};
struct data_unit {
	data_unit(const data_unit&) {}
	data_unit(data_unit&&) noexcept {}
	~data_unit() {}

	data_unit& operator=(data_unit&&) noexcept {
		return *this;
	}

	dynamic_cstring name;
	strong_ref<type> data_type;
	mem_handler mh;
};
struct table {
	dynamic_array<data_unit> aaa;
};
*/
using func = int(*)(void*);//TODO



struct table;
struct unit {
	static const size_t char_num = sizeof(ptrdiff_t);
	enum type {
		NIL,
		INT,
		FP,
		FUNC,
		VOID,
		CSTR
	};
	//MUST BE MANUALY CONTROLLED
	union data {
		data() {}
		~data() {}
		ptrdiff_t	int_val;				//integer
		floatpoint	float_val;				//float point
		func		func_val;				//function
		mem_handler	mem_val;				//void block
		mem_wrapper<dynamic_cstring> cstr;	//cstring
	};
	protected:
		type _type;
		data _data;
	public:
		unit() : _type(NIL) {}
		explicit unit(const ptrdiff_t i_val) : _type(INT) {
			_data.int_val = i_val;
		}
		explicit unit(const floatpoint fp_val) : _type(FP) {
			_data.float_val = fp_val;
		}
		explicit unit(const func f_val) : _type(FUNC) {
			_data.func_val = f_val;
		}
		explicit unit(mem_handler&& m_val) : _type(VOID) {
			_data.int_val = 0; //create nullptr
			_data.mem_val = std::move(m_val);
		}
		explicit unit(const dynamic_cstring& cstr) : _type(CSTR) {
			new (&_data.cstr) mem_wrapper<dynamic_cstring>(cstr);
		}

		unit(const unit& other) : _type(other._type) {
			if (other._type == VOID) {
				throw exception(0, "void can`t be copied!");
			}
			//we can use copy memory by type
			_data.int_val = other._data.int_val;
		}
		unit(unit&& other) noexcept : _type(other._type) {
			other._type = NIL;
			//we can use copy memory by type
			_data.int_val = other._data.int_val;
		}
		~unit() {
			switch (_type) {
				case unit::VOID:
					_data.mem_val.~mem_handler();
					break;
				case unit::CSTR:
					_data.cstr.~mem_wrapper<dynamic_cstring>();
					break;
				default:
					break;
			}
		}

		unit& operator=(const unit& other) {
			if (this == &other) {
				return *this;
			}
			switch (other._type) {
				case VOID:
					throw exception(0, "void can`t be copied!");
				case CSTR:
					if (_type == VOID) {
						throw exception(0, "void can`t be abandoned!");
					}
					if (_type != CSTR) {
						new (&_data.cstr) mem_wrapper<dynamic_cstring>(other._data.cstr.get());
					}
					else {
						_data.cstr.get() = other._data.cstr.get();
					}
					break;
				default:
					//we can use copy memory by type
					_data.int_val = other._data.int_val;
					break;
			}
			_type = other._type;
			return *this;
		}
		unit& operator=(unit&& other) noexcept {
			if (this == &other) {
				return *this;
			}
			//we can use copy memory by type
			std::swap(_data.int_val, other._data.int_val);
			std::swap(_type, other._type);
			return *this;
		}


		template<typename T>
		T& get();
		template<>
		ptrdiff_t& get() {
			if (_type == INT) {
				return _data.int_val;
			}
			throw exception(0, "wrong return type");
		}
		template<>
		floatpoint& get() {
			if (_type == FP) {
				return _data.float_val;
			}
			throw exception(0, "wrong return type");
		}
		template<>
		func& get() {
			if (_type == FUNC) {
				return _data.func_val;
			}
			throw exception(0, "wrong return type");
		}
		template<>
		mem_handler& get() {
			if (_type == VOID) {
				return _data.mem_val;
			}
			throw exception(0, "wrong return type");
		}
		template<>
		dynamic_cstring& get() {
			if (_type == CSTR) {
				return _data.cstr.get();
			}
			throw exception(0, "wrong return type");
		}
		//TODO for const
};
struct table_unit {
	unit u;
	dynamic_cstring name;
};
struct table {
	size_t index_offset;
	dynamic_array<table_unit> t_units;
	table_unit& operator[](size_t index) {
		return t_units[index + index_offset];
		//check for out of range?
	}
	table_unit& operator[](dynamic_cstring name) {
		for (table_unit& tu : t_units) {
			if (tu.name == name) {
				return tu;
			}
		}
		//throw???
	}
};

