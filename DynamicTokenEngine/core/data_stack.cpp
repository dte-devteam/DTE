#include "data_stack.hpp"
#include "memory/memory.hpp"
using namespace dte_utils;
data_stack::data_stack(size_t stack_size) {
	char* begin = tmalloc<char>(stack_size);
	blocks.emplace_back(begin + stack_size, begin);
}
data_stack::data_stack(data_stack&& other) noexcept : blocks(std::move(other.blocks)) {
	other.blocks.emplace_back(nullptr, nullptr);	//other now frees nothing
}
data_stack::~data_stack() {
	free(blocks[0].physical_end);
}

data_stack& data_stack::operator=(data_stack&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(blocks, other.blocks);
	return *this;
}

void* data_stack::push_real(size_t block_size) {
	if (block_size > get_memory_left()) {
		throw exception(0, "stack overflow");
	}
	blocks.emplace_back(
		blocks.back().physical_end,
		blocks.back().physical_end + block_size
	);
	return blocks.back().virtual_begin;
}
void data_stack::push_virt(void* virt_block) {
	blocks.emplace_back(
		static_cast<char*>(virt_block),
		blocks.back().physical_end
	);
}
void data_stack::pop() {
	blocks.pop_back();
}
void data_stack::pop(size_t block_num) {
	if (block_num > get_block_num()) {
		throw exception(0, "stack heap is untouchable");
	}
	blocks.pop_back(block_num);
}
void data_stack::clear() {
	pop(get_block_num());
}
size_t data_stack::get_block_num() const {
	return blocks.get_used() - 1; //- 1 because [0] is stack heap
}
size_t data_stack::get_block_size(size_t index) const {
	return blocks[index + 1].physical_end - blocks[index].physical_end;
}
size_t data_stack::get_memory_left() const {
	return blocks[0].virtual_begin - blocks.back().physical_end;
}
size_t data_stack::get_allocated() const {
	return blocks[0].virtual_begin - blocks[0].physical_end;
}
void* data_stack::operator[](size_t index) const {
	return blocks[index + 1].virtual_begin;	//+ 1 because [0] is stack heap
}