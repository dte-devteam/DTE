#pragma once
#include "dynamic_stack.hpp"
namespace dte_utils {
	template<allocatable T, template<allocatable> typename A = allocator>
	requires is_allocator<A, T> && std::is_move_assignable_v<T>
	struct dynamic_array : dynamic_stack<T, A> {
		using size_type = dynamic_stack<T, A>::size_type;
		using type = dynamic_stack<T, A>::type;
		using const_type = dynamic_stack<T, A>::const_type;
		using pointer = dynamic_stack<T, A>::pointer;
		using const_pointer = dynamic_stack<T, A>::const_pointer;
		using dynamic_stack<T, A>::dynamic_stack;
		protected:
			A<T> _provide_spaced_buffer(pointer gap_pointer, size_type gap_size) const {
				A<T> new_allocator(this->get_allocated()+1000);
				size_type first_section_size = gap_pointer - this->begin();
				array_to_array(
					static_cast<pointer>(new_allocator), 
					this->begin(),
					first_section_size
				);
				array_to_array(
					static_cast<pointer>(new_allocator) + first_section_size + gap_size,
					this->begin() + first_section_size,
					this->get_used() - first_section_size
				);
				return new_allocator;
			}
		public:
			template<copy_constructible<T> U>
			void insert(pointer pos, const U& value) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() == this->get_allocated()) {
					this->_allocated = this->_extend_by_el();
					A<T> new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(static_cast<pointer>(new_allocator) + (pos - this->begin()), value);
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					pointer over_pos = this->end();
					place_at(over_pos, value);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++this->_used;
			}
			template<move_constructible<T> U>
			void insert(pointer pos, U&& value) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() == this->get_allocated()) {
					this->_allocated = this->_extend_by_el();
					A<T> new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(static_cast<pointer>(new_allocator) + (pos - this->begin()), value);
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					pointer over_pos = this->end();
					place_at(over_pos, value);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++this->_used;
			}
			
			template<copy_constructible<T> U>
			void insert(pointer pos, const U& value, size_type num) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() + num > this->get_allocated()) {
					this->_allocated = this->get_used() + num;
					A<T> new_allocator = this->_provide_spaced_buffer(pos, num);
					pointer target = static_cast<pointer>(new_allocator) + (pos - this->begin());
					while (num) {
						place_at(target, value);
						--num;
						++target;
					}
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
					this->_used = this->get_allocated();
				}
				else {
					pointer over_pos = this->end() + num;
					while (this->end() != over_pos) {
						place_at(--over_pos, value);
					}
					while (num) {
						over_pos = this->end();
						while (over_pos != pos) {
							std::swap(*--over_pos, *over_pos);
						}
						++pos;
						++this->_used;
						--num;
					}
				}
			}
			template<copy_constructible<T> U>
			void insert(pointer pos, const U* first, const U* last) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				size_type num = last - first;
				if (this->get_used() + num > this->get_allocated()) {
					this->_allocated = this->get_used() + num;
					A<T> new_allocator = _provide_spaced_buffer(pos, num);
					pointer target = static_cast<pointer>(new_allocator) + (pos - this->begin());
					while (first != last) {
						place_at(target, *first);
						++first;
						++target;
					}
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
					this->_used = this->get_allocated();
				}
				else {
					pointer over_pos = this->end() + num;
					while (this->end() != over_pos) {
						place_at(--over_pos, *--last);
					}
					while (num) {
						over_pos = this->end();
						while (over_pos != pos) {
							std::swap(*--over_pos, *over_pos);
						}
						++pos;
						++this->_used;
						--num;
					}
				}
			}
			template<copy_constructible<T> U>
			void insert(pointer pos, std::initializer_list<U> il) {
				insert(pos, il.begin(), il.end());
			}

			template<typename ...Args>
			void emplace(pointer pos, Args&&... args) requires std::is_constructible_v<T, Args&&...> {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() == this->get_allocated()) {
					this->_allocated = this->_extend_by_el();
					A<T> new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(static_cast<pointer>(new_allocator) + (pos - this->begin()), std::forward<Args>(args)...);
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					pointer over_pos = this->end();
					place_at(over_pos, std::forward<Args>(args)...);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++this->_used;
			}

			void erase(pointer pos) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				while (pos != this->end()) {
					std::swap(*++pos, *pos);
				}
				--this->_used;
				if constexpr (!std::is_trivially_destructible_v<T>) {
					this->end()->~T();
				}
			}
			void erase(pointer first, pointer last) {
				if (first > last) {
					throw invalid_range();
				}
				if (first < this->begin() || last > this->end()) {
					throw out_of_range();
				}
				size_type size = last - first;
				while (last != this->end()) {
					std::swap(*first, *last);
					++first;
					++last;
				}
				if constexpr (!std::is_trivially_destructible_v<T>) {
					destruct_range(this->end() - size, this->end());
				}
				this->_used -= size;
			}

			//unordered removal (faster, but order breaks)
			void remove(pointer pos) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				--this->_used;
				std::swap(*pos, *(this->end()));
				this->end()->~T();
			}
			//is effective in larger arrays and smaller removal
			void remove(pointer first, pointer last) {
				//TODO: if first/last is out or range
				if (last == this->end()) {
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destruct_range(first, this->end());
					}
					this->_used -= last - first;
				}
				else {
					pointer over_pos = this->end();
					this->_used -= last - first;
					while (last != first) {
						std::swap(*--last, *--over_pos);
						over_pos->~T();
					}
				}
			}
	};
}
