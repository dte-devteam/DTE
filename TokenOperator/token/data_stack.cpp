#include "data_stack.hpp"
#include "memory/memory.hpp"
using namespace dte_utils;
using namespace dte_token;
data_stack::data_stack(size_type stack_size) {
	char* begin = tmalloc<char>(stack_size);
	blocks.emplace_back(begin + stack_size, begin, nullptr);
}
data_stack::data_stack(data_stack&& other) noexcept : blocks(std::move(other.blocks)) {
	other.blocks.emplace_back(nullptr, nullptr, nullptr);	//other now frees nothing
}
data_stack::~data_stack() {
	for (size_type i = 1; i < blocks.get_used(); ++i) {
		if (blocks[i].destr) {
			blocks[i].destr(blocks[i].virtual_begin);
		}
	}
	free(blocks[0].physical_end);
}

data_stack& data_stack::operator=(data_stack&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(blocks, other.blocks);
	return *this;
}

data_stack::pointer data_stack::push_real(size_type block_size, destructor* destr) {
	if (block_size > get_memory_left()) {
		throw exception(0, "stack overflow");
	}
	blocks.emplace_back(
		blocks.back().physical_end,
		blocks.back().physical_end + block_size,
		destr
	);
	return blocks.back().virtual_begin;
}
void data_stack::push_virt(pointer virt_block) {
	blocks.emplace_back(
		static_cast<char*>(virt_block),
		blocks.back().physical_end,
		nullptr
	);
}
void data_stack::pop() {
	if (get_block_num()) {
		blocks.pop_back();
		return;
	}
	throw exception(0, "stack heap is untouchable");
}
void data_stack::pop(size_type block_num) {
	if (block_num > get_block_num()) {
		throw exception(0, "stack heap is untouchable");
	}
	blocks.pop_back(block_num);
}
void data_stack::clear() {
	pop(get_block_num());
}
data_stack::size_type data_stack::get_block_num() const {
	return blocks.get_used() - 1; //- 1 because [0] is stack heap
}
data_stack::size_type data_stack::get_block_size(size_type index) const {
	return blocks[index + 1].physical_end - blocks[index].physical_end;
}
data_stack::size_type data_stack::get_memory_left() const {
	return blocks[0].virtual_begin - blocks.back().physical_end;
}
data_stack::size_type data_stack::get_allocated() const {
	return blocks[0].virtual_begin - blocks[0].physical_end;
}
data_stack::pointer data_stack::operator[](size_type index) const {
	return blocks[index + 1].virtual_begin;	//+ 1 because [0] is stack heap
}
data_stack::pointer data_stack::back() const {
	return blocks.back().virtual_begin;
}