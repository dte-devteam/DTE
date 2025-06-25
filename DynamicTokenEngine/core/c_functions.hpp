#include "token/data_stack.hpp"
#include "token/unit.hpp"



#include <fstream>
namespace dte_core {
	void ifstr_destr(void* block) {
		static_cast<std::ifstream*>(block)->~basic_ifstream();
	}
	//helpers
	inline size_t frame_size(dte_token::data_stack& ds, size_t offset) {
		return ds.get_block_num() - offset;
	}
	//
	inline size_t push_virtual(dte_token::data_stack& ds, size_t offset) {
		ds.push_virt(ds[offset]);
		return 1;
	}


	//file
	inline size_t create_ifstream(dte_token::data_stack& ds, size_t offset) {
		new (ds.push_real(sizeof(std::ifstream), ifstr_destr)) std::ifstream();
		return 1;
	}
	inline size_t open_file(dte_token::data_stack& ds, size_t offset) {
		ptrdiff_t open_mode = std::ios_base::in;
		if (frame_size(ds, offset) > 2) {
			open_mode = static_cast<dte_token::unit*>(ds[offset + 2])->get_int()[0];
		}
		static_cast<std::ifstream*>(
			ds[offset]
		)->open(static_cast<dte_token::unit*>(ds[offset + 1])->get_cstr().begin(), open_mode);
		return 1;
	}
	inline size_t close_file(dte_token::data_stack& ds, size_t offset) {
		static_cast<std::ifstream*>(
			ds[offset]
		)->close();
		return 1;
	}
	inline size_t read_line(dte_token::data_stack& ds, size_t offset) {
		std::ifstream& ifstr = *static_cast<std::ifstream*>(ds[offset]);
		dte_utils::dynamic_cstring& str = static_cast<dte_token::unit*>(ds[offset + 1])->get_cstr();
		str.clear();
		for (char c = '\0'; ifstr.get(c) && c != '\n';) {
			str.push_back(c);
		}
		if (ifstr.eof() && str.get_used()) {
			ifstr.clear();
		}
		return 1;
	}
}