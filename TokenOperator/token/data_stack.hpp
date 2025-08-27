#pragma once
#include "memory/dynamic_stack.hpp"
#include "types.hpp"
namespace dte_token {
	struct data_stack {
		using size_type = size_t;
		using pointer = void*;
		struct block {
			char* virtual_begin;
			char* physical_end;
			destructor* destr;
		};
		protected:
			dte_utils::dynamic_stack<block> blocks;
		public:
			data_stack(size_type stack_size);
			data_stack(const data_stack&) = delete;
			data_stack(data_stack&& other) noexcept;
			data_stack& operator=(const data_stack&) = delete;
			data_stack& operator=(data_stack&& other) noexcept;
			~data_stack();
			pointer push_real(size_type block_size, destructor* destr);
			pointer push_real(size_type block_size, size_type alignment, destructor* destr);
			void push_virt(pointer virt_block);
			template<bool is_fail_safe = false>
			void pop() {
				if constexpr (!is_fail_safe) {
					if (!get_block_num()) {
						throw exception(0, "stack heap is untouchable");
					}
				}
				if (blocks.back<true>().destr) {
					blocks.back<true>().destr(back());
				}
				blocks.pop_back<true>();
			}
			template<bool is_fail_safe = false>
			void pop(size_type block_num) {
				if constexpr (!is_fail_safe) {
					if (block_num > get_block_num()) {
						throw exception(0, "stack heap is untouchable");
					}
				}
				while (block_num) {
					pop<true>();
					--block_num;
				}
			}
			void clear();
			size_type get_block_num() const noexcept;
			size_type get_block_size(size_type index) const;
			size_type get_memory_left() const noexcept;
			size_type get_allocated() const noexcept;
			pointer operator[](size_type index) const;
			pointer back() const noexcept;
	};
}