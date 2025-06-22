#include "token/data_stack.hpp"
#include "token/unit.hpp"



#include <fstream>
namespace dte_core {
	//helpers
	inline size_t frame_size(dte_token::data_stack& ds, size_t offset) {
		return ds.get_block_num() - offset;
	}
	inline void push_void_unit(dte_token::data_stack& ds, void* v) {
		new (ds.push_real(sizeof(dte_token::unit))) dte_token::unit(
			dte_utils::strong_ref<dte_utils::mem_handler>(
				dte_utils::cnew<dte_utils::mem_handler>(v)
			)
		);
	}
	inline void* get_unit_void(dte_token::data_stack& ds, size_t offset) {
		return static_cast<dte_token::unit*>(ds[offset])->get_void()->operator void*();
	}
	inline dte_utils::dynamic_cstring& get_unit_cstr(dte_token::data_stack& ds, size_t offset) {
		return static_cast<dte_token::unit*>(ds[offset])->get_cstr();
	}
	inline dte_utils::static_array<ptrdiff_t, 3>& get_unit_int(dte_token::data_stack& ds, size_t offset) {
		return static_cast<dte_token::unit*>(ds[offset])->get_int();
	}


	//stack
	inline size_t push_virt(dte_token::data_stack& ds, size_t offset) {
		ds.push_virt(ds[offset]);
		return 1;
	}
	inline size_t pop_dirty(dte_token::data_stack& ds, size_t offset) {
		ds.pop(ds.get_block_num() - offset);
		return 1;
	}
	inline size_t pop_clean(dte_token::data_stack& ds, size_t offset) {
		dte_token::unit* pointer = static_cast<dte_token::unit*>(ds[offset]);
		dte_utils::destruct_range(pointer, pointer + ds.get_block_num() - offset);
		return pop_dirty(ds, offset);
	}

	//file
	inline size_t create_ifstream(dte_token::data_stack& ds, size_t offset) {
		push_void_unit(ds, dte_utils::cnew<std::ifstream>());
		return 1;
	}
	inline size_t release_ifstream(dte_token::data_stack& ds, size_t offset) {
		static_cast<std::ifstream*>(
			get_unit_void(ds, offset)
		)->~basic_ifstream();
		return 1;
	}
	inline size_t open_file(dte_token::data_stack& ds, size_t offset) {
		size_t open_mode = std::ios_base::in;
		if (frame_size(ds, offset) > 2) {
			open_mode = get_unit_int(ds, offset + 2)[0];
		}
		static_cast<std::ifstream*>(
			get_unit_void(ds, offset)
		)->open(get_unit_cstr(ds, offset + 1).begin(), open_mode);
		return 1;
	}
	inline size_t close_file(dte_token::data_stack& ds, size_t offset) {
		static_cast<std::ifstream*>(
			get_unit_void(ds, offset)
		)->close();
		return 1;
	}
	inline size_t read_line(dte_token::data_stack& ds, size_t offset) {
		std::ifstream& ifstr = *static_cast<std::ifstream*>(get_unit_void(ds, offset));
		dte_utils::dynamic_cstring& str = get_unit_cstr(ds, offset + 1);
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