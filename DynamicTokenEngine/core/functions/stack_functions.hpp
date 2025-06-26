#pragma once
#include "token/data_stack.hpp"
namespace dte_core {
	inline size_t get_frame_size(dte_token::data_stack& ds, size_t offset) {
		return ds.get_block_num() - offset;
	}
	inline void push_virtual(dte_token::data_stack& ds, size_t offset) {
		ds.push_virt(ds[offset]);
	}
	template<typename T>
	inline T* get(dte_token::data_stack& ds, size_t offset) {
		return static_cast<T*>(ds[offset]);
	}
}