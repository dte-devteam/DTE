#pragma once
#include "token/data_stack.hpp"
#include "token/semi_pointer.hpp"
#include "core/error_codes.hpp"
#include "core/error_desc.hpp"
namespace dte_core {
	inline size_t get_frame_size(dte_token::data_stack& ds, size_t offset) {
		return ds.get_block_num() - offset;
	}
	template<typename T, bool check_size = true>
	inline T* get(dte_token::data_stack& ds, size_t offset) {
		if constexpr (check_size) {
			if (ds.get_block_size(offset) != sizeof(T)) {
				throw exception(
					dte_error::full_error_code(dte_error::error_section::STACK, dte_error::stack::STACK_GET_UNEXPECTED_SIZE),
					dte_error::STACK_CODE[dte_error::stack::STACK_GET_UNEXPECTED_SIZE]
				);
			}
		}
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
}