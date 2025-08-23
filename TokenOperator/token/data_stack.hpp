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
			data_stack(const data_stack& other) = delete;
			data_stack(data_stack&& other) noexcept;
			data_stack& operator=(const data_stack& other) = delete;
			data_stack& operator=(data_stack&& other) noexcept;
			~data_stack();
			pointer push_real(size_type block_size, size_type alignment, destructor* destr);
			void push_virt(pointer virt_block);
			void pop();
			void pop(size_type block_num);
			void clear();
			size_type get_block_num() const;
			size_type get_block_size(size_type index) const;
			size_type get_memory_left() const;
			size_type get_allocated() const;
			pointer operator[](size_type index) const;
			pointer back() const;
	};
}