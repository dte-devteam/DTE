#pragma once
#include "memory/dynamic_stack.hpp"
namespace dte_token {
	struct data_stack {
		struct block {
			char* virtual_begin;
			char* physical_end;
		};
		protected:
			dte_utils::dynamic_stack<block> blocks;
		public:
			data_stack(size_t stack_size);
			data_stack(const data_stack& other) = delete;
			data_stack(data_stack&& other) noexcept;
			data_stack& operator=(const data_stack& other) = delete;
			data_stack& operator=(data_stack&& other) noexcept;
			~data_stack();
			void* push_real(size_t block_size);
			void push_virt(void* virt_block);
			void pop();
			void pop(size_t block_num);
			void clear();
			size_t get_block_num() const;
			size_t get_block_size(size_t index) const;
			size_t get_memory_left() const;
			size_t get_allocated() const;
			void* operator[](size_t index) const;
			void* back();
	};
}