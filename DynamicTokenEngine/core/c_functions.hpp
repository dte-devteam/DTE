#include "token/data_stack.hpp"
#include "token/unit.hpp"
namespace dte_core {
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
}