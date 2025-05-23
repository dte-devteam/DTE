#pragma once
#include "memory/dynamic_stack.hpp"
#include "memory/memory.hpp"
using namespace dte_utils;
struct function_stack {
	struct block {
		char* virtual_begin;
		char* physical_end;
	};
	protected:
		dynamic_stack<block> blocks;
		char* stack_end;
	public:
		function_stack(size_t stack_size);
		~function_stack();
		void push_real(size_t block_size);
		void push_virt(void* virt_block);
		void pop();
		void pop(size_t block_num);
		void clear();
		size_t function_stack::get_size() const;
		void* function_stack::operator[](size_t index) const;
};