#pragma once
#include "dynamic_stack.hpp"

#include <iostream>
namespace dte_utils {
	template<typename T, template<typename> typename A = allocator>
	struct dynamic_array : dynamic_stack<T, A> {
		using stack				= dynamic_stack<T, A>;
		using alloc_hand		= stack::alloc_hand;
		using alloc_inst		= stack::alloc_inst;
		using size_type			= stack::size_type;
		using type				= stack::type;
		using const_type		= stack::const_type;
		using iterator			= stack::iterator;
		using const_iterator	= stack::const_iterator;

		using stack::dynamic_stack;
		protected:
			//TODO: second array_to_array can be optimised by b_iterator
			alloc_inst _provide_spaced_buffer(const iterator& gap_pointer, size_type gap_size) const {
				alloc_inst new_allocator(this->get_allocated() + gap_size);
				//TODO: size can be negative
				size_type first_section_size = gap_pointer - this->begin();
				array_to_array(
					this->begin(),
					this->begin() + first_section_size,
					new_allocator.operator iterator()
				);
				array_to_array(
					this->begin() + first_section_size,
					this->end(),
					new_allocator.operator iterator() + first_section_size + gap_size
				);
				return new_allocator;
			}
		public:
			template<typename U, template<typename> typename It>
			void insert(const It<type>& pos, const U& value) 
			requires is_iteroid_v<It, type> {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() == this->get_allocated()) {
					this->_allocated = this->_extend_by_el();
					alloc_inst new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(new_allocator.operator iterator() + (pos - this->begin()), value);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					iterator over_pos = this->end();
					place_at(over_pos, value);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++this->_used;
			}
			template<typename U, template<typename> typename It>
			void insert(const It<type>& pos, U&& value) 
			requires is_iteroid_v<It, type> {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() == this->get_allocated()) {
					this->_allocated = this->_extend_by_el();
					alloc_inst new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(new_allocator.operator iterator() + (pos - this->begin()), std::move(value));
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					iterator over_pos = this->end();
					place_at(over_pos, std::move(value));
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++this->_used;
			}
			
			//TODO: check, if other iterators (pos) can be used
			template<typename U>
			void insert(iterator pos, const U& value, size_type num) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() + num > this->get_allocated()) {
					this->_allocated = this->get_used() + num;
					alloc_inst new_allocator = this->_provide_spaced_buffer(pos, num);
					iterator target = new_allocator.operator iterator() + (pos - this->begin());
					while (num) {
						place_at(target, value);
						--num;
						++target;
					}
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
					this->_used = this->get_allocated();
				}
				else {
					iterator over_pos = this->end() + num;
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
			//TODO: check, if other iterators (pos) can be used
			template<typename U, template<typename> typename ItT, template<typename> typename ItU>
			void insert(iterator pos, const ItT<U>& first, ItU<U> last) 
			requires is_iteroid_v<ItT, U> && is_iteroid_v<ItU, U> {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				//TODO: check if num can be negative
				size_type num = last - first;
				if (this->get_used() + num > this->get_allocated()) {
					this->_allocated = this->get_used() + num;
					alloc_inst new_allocator = _provide_spaced_buffer(pos, num);
					iterator target = new_allocator.operator iterator() + (pos - this->begin());
					array_to_array(
						first,
						last,
						target
					);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
					this->_used = this->get_allocated();
				}
				else {
					iterator over_pos = this->end() + num;
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
			//TODO: check, if other iterators (pos) can be used
			template<template<typename> typename It>
			void insert(const iterator& pos, std::initializer_list<type> il) {
				insert(pos, iterator(il.begin()), iterator(il.end()));
			}
			//TODO: check, if other iterators (pos) can be used
			template<typename ...Args>
			void emplace(const iterator& pos, Args&&... args) {
				if (pos < this->begin() || pos > this->end()) {
					throw out_of_range();
				}
				if (this->get_used() == this->get_allocated()) {
					this->_allocated = this->_extend_by_el();
					alloc_inst new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(new_allocator.operator iterator() + (pos - this->begin()), std::forward<Args>(args)...);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(this->begin(), this->end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					iterator over_pos = this->end();
					place_at(over_pos, std::forward<Args>(args)...);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++this->_used;
			}

			//TODO: check, if other iterators (pos) can be used
			template<bool is_fail_safe = false>
			void erase(iterator pos)
			noexcept(
				std::is_nothrow_destructible_v<type> &&
				std::is_nothrow_swappable_v<type> &&
				is_fail_safe
			) {
				if constexpr (!is_fail_safe) {
					if (pos < this->begin() || pos > this->end()) {
						throw out_of_range();
					}
				}
				--this->_used;
				while (pos != this->end()) {
					std::swap(*++pos, *pos);
				}
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destuct_at(this->end());
				}
			}
			//TODO: check, if other iterators (pos) can be used
			template<bool is_fail_safe = false>
			void erase(iterator first, iterator last)
			noexcept(
				std::is_nothrow_destructible_v<type>&&
				std::is_nothrow_swappable_v<type>&&
				is_fail_safe
			) {
				if constexpr (!is_fail_safe) {
					if (first > last) {
						throw invalid_range();
					}
					if (first < this->begin() || last > this->end()) {
						throw out_of_range();
					}
				}
				size_type size = last - first;
				while (last != this->end()) {
					std::swap(*first, *last);
					++first;
					++last;
				}
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(this->end() - size, this->end());
				}
				this->_used -= size;
			}

			//unordered removal (faster, but order breaks)
			template<bool is_fail_safe = false>
			void remove(const iterator& pos) 
			noexcept(
				std::is_nothrow_destructible_v<type>&&
				std::is_nothrow_swappable_v<type>&&
				is_fail_safe
			) {
				if constexpr (!is_fail_safe) {
					if (pos < this->begin() || pos > this->end()) {
						throw out_of_range();
					}
				}
				--this->_used;
				std::swap(*pos, *(this->end()));
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destuct_at(this->end());
				}
			}
			//TODO: check, if other iterators (pos) can be used
			//is effective in larger arrays and smaller removal
			template<bool is_fail_safe = false>
			void remove(const iterator& first, iterator last)
			noexcept(
				std::is_nothrow_destructible_v<type>&&
				std::is_nothrow_swappable_v<type>&&
				is_fail_safe
			) {
				if constexpr (!is_fail_safe) {
					if (first > last) {
						throw invalid_range();
					}
					if (first < this->begin() || last > this->end()) {
						throw out_of_range();
					}
				}
				if (last == this->end()) {
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(first, this->end());
					}
					this->_used -= last - first;
				}
				else {
					iterator over_pos = this->end();
					this->_used -= last - first;
					while (last != first) {
						std::swap(*--last, *--over_pos);
						if constexpr (!std::is_trivially_destructible_v<type>) {
							destuct_at(over_pos);
						}
					}
				}
			}
	};
}
