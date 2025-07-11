#pragma once
#include "token/data_stack.hpp"
namespace dte_core {
	inline size_t get_frame_size(dte_token::data_stack& ds, size_t offset) {
		return ds.get_block_num() - offset;
	}
	template<typename T>
	inline T* get(dte_token::data_stack& ds, size_t offset) {
		return static_cast<T*>(ds[offset]);
	}

	//Static Arg
	inline size_t push_sa_virtual(dte_token::data_stack& ds, size_t offset) {
		ds.push_virt(ds[offset]);
		return 0;
	}
	//Dynamic Arg
	inline size_t push_da_virtual(dte_token::data_stack& ds, size_t offset) {
		ds.push_virt(ds[offset - *get<size_t>(ds, offset)]);
		return 0;
	}

	inline size_t cond_dec(dte_token::data_stack& ds, size_t offset) {
		return --*get<size_t>(ds, offset) ? 0 : 1;
	}
}