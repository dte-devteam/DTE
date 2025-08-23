#pragma once
#include "stack_functions.hpp"
#include <fstream>
#include "memory/dynamic_string.hpp"
namespace dte_core {
	struct ifstr_args {
		const char* _Filename;
		std::ios_base::openmode _Mode = std::ios_base::in;
		int _Prot = std::ios_base::_Default_open_prot;
	};


	//destructors
	inline void ifstr_args_destructor(void* block) {
		static_cast<ifstr_args*>(block)->~ifstr_args();
	}
	inline void ifstream_destructor(void* block) {
		static_cast<std::ifstream*>(block)->~basic_ifstream();
	}
	inline void dynamic_cstring_destructor(void* block) {
		static_cast<dte_utils::dynamic_cstring*>(block)->~dynamic_string();
	}


	inline size_t create_ifstream(dte_token::data_stack& ds, const dte_token::semi_pointer::data& spd) {
		if (spd.ptr) {
			const ifstr_args& args = *static_cast<const ifstr_args*>(spd.ptr);
			new (ds.push_real(sizeof(std::ifstream), alignof(std::ifstream), ifstream_destructor)) std::ifstream(args._Filename, args._Mode, args._Prot);
			return 0;
		}
		new (ds.push_real(sizeof(std::ifstream), alignof(std::ifstream), ifstream_destructor)) std::ifstream();
		return 0;
	}
	inline size_t create_cstr(dte_token::data_stack& ds, const dte_token::semi_pointer::data& spd) {
		if (spd.ptr) {
			const dte_utils::dynamic_cstring& arg = *static_cast<const dte_utils::dynamic_cstring*>(spd.ptr);
			new (ds.push_real(sizeof(dte_utils::dynamic_cstring), alignof(dte_utils::dynamic_cstring), dynamic_cstring_destructor)) dte_utils::dynamic_cstring(arg);
			return 0;
		}
		new (ds.push_real(sizeof(dte_utils::dynamic_cstring), alignof(dte_utils::dynamic_cstring), dynamic_cstring_destructor)) dte_utils::dynamic_cstring();
		return 0;
	}



	inline size_t open_file(dte_token::data_stack& ds, const dte_token::semi_pointer::data& spd) {
		std::ios_base::openmode open_mode = std::ios_base::in;
		//if (get_frame_size(ds, offset) > 2) {
		//	open_mode = get<dte_token::unit>(ds, offset + 2)->get_int()[0];
		//}
		get<std::ifstream>(ds, spd.offset)->open(get<dte_utils::dynamic_cstring>(ds, spd.offset + 1)->begin(), open_mode);
		return 0;
	}
	inline size_t close_file(dte_token::data_stack& ds, const dte_token::semi_pointer::data& spd) {
		get<std::ifstream>(ds, spd.offset)->close();
		return 0;
	}
	inline size_t read_line(dte_token::data_stack& ds, const dte_token::semi_pointer::data& spd) {
		std::ifstream* ifstr = get<std::ifstream>(ds, spd.offset);
		dte_utils::dynamic_cstring& str = *get<dte_utils::dynamic_cstring>(ds, spd.offset + 1);
		str.native_pop(str.get_used());
		for (char c = 0; ifstr->get(c) && c != '\n';) {
			str.native_push(c);
		}
		if (ifstr->eof() && str.get_used()) {
			ifstr->clear();
		}
		str.native_push(0);
		return 0;
	}
}