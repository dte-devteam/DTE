#pragma once
#include "stack_functions.hpp"
#include "token/unit.hpp"
#include <fstream>
namespace dte_core {
	//destructors
	inline void ifstream_destructor(void* block) {
		static_cast<std::ifstream*>(block)->~basic_ifstream();
	}


	inline size_t create_ifstream(dte_token::data_stack& ds, size_t offset) {
		new (ds.push_real(sizeof(std::ifstream), ifstream_destructor)) std::ifstream();
		return 0;
	}
	inline size_t open_file(dte_token::data_stack& ds, size_t offset) {
		ptrdiff_t open_mode = std::ios_base::in;
		//if (get_frame_size(ds, offset) > 2) {
		//	open_mode = get<dte_token::unit>(ds, offset + 2)->get_int()[0];
		//}
		get<std::ifstream>(ds, offset)->open(get<dte_utils::dynamic_cstring>(ds, offset + 1)->begin(), open_mode);
		return 0;
	}
	inline size_t close_file(dte_token::data_stack& ds, size_t offset) {
		get<std::ifstream>(ds, offset)->close();
		return 0;
	}
	inline size_t read_line(dte_token::data_stack& ds, size_t offset) {
		std::ifstream* ifstr = get<std::ifstream>(ds, offset);
		dte_utils::dynamic_cstring& str = *get<dte_utils::dynamic_cstring>(ds, offset + 1);
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