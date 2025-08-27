#include "data_stack.hpp"
#include "memory/memory.hpp"
using namespace dte_utils;
using namespace dte_token;
data_stack::data_stack(size_type stack_size) {
	char* const begin = tmalloc<char>(stack_size);
	blocks.emplace_back(begin + stack_size, begin, nullptr);
}
data_stack::data_stack(data_stack&& other) noexcept : blocks(std::move(other.blocks)) {
	other.blocks.emplace_back(nullptr, nullptr, nullptr);	//other now frees nothing
}
data_stack::~data_stack() {
	clear();
	free(blocks.operator[]<true>(0).physical_end);
}

data_stack& data_stack::operator=(data_stack&& other) noexcept {
	if (this == &other) {
		return *this;
	}
	std::swap(blocks, other.blocks);
	return *this;
}
data_stack::pointer data_stack::push_real(size_type block_size, destructor* destr) {
	blocks.emplace_back(
		blocks.back<true>().physical_end,
		blocks.back<true>().physical_end + block_size,
		destr
	);
	return blocks.back<true>().virtual_begin;
}
data_stack::pointer data_stack::push_real(size_type block_size, size_type alignment, destructor* destr) {
	size_type virtual_begin = reinterpret_cast<size_type>(blocks.back<true>().physical_end) + alignment - 1;
	virtual_begin &= ~(alignment - 1);
	char* physical_end = reinterpret_cast<char*>(virtual_begin) + block_size;
	if (physical_end > blocks[0].virtual_begin) {
		throw exception(0, "stack overflow");
	}
	blocks.emplace_back(
		reinterpret_cast<char*>(virtual_begin),
		physical_end,
		destr
	);
	return blocks.back<true>().virtual_begin;
}
void data_stack::push_virt(pointer virt_block) {
	blocks.emplace_back(
		static_cast<char*>(virt_block),
		blocks.back<true>().physical_end,
		nullptr
	);
}
void data_stack::clear() {
	pop<true>(get_block_num());
}
data_stack::size_type data_stack::get_block_num() const noexcept {
	return blocks.get_used() - 1; //- 1 because [0] is stack heap
}
data_stack::size_type data_stack::get_block_size(size_type index) const {
	return blocks[index + 1].physical_end - blocks[index].physical_end;
}
data_stack::size_type data_stack::get_memory_left() const noexcept {
	return blocks.operator[]<true>(0).virtual_begin - blocks.back<true>().physical_end;
}
data_stack::size_type data_stack::get_allocated() const noexcept {
	return blocks.operator[]<true>(0).virtual_begin - blocks.operator[]<true>(0).physical_end;
}
data_stack::pointer data_stack::operator[](size_type index) const {
	return blocks[index + 1].virtual_begin;	//+ 1 because [0] is stack heap
}
data_stack::pointer data_stack::back() const noexcept {
	return blocks.back<true>().virtual_begin;
}