#pragma once
#include "dynamic_stack.hpp"
namespace dte_utils {
	template<non_void T, template<typename> typename A = allocator>
	struct dynamic_array : dynamic_stack<T, A> {
		using stack				= typename dynamic_stack<T, A>;
		using alloc_hand		= typename stack::alloc_hand;
		using alloc_inst		= typename stack::alloc_inst;
		using size_type			= typename stack::size_type;
		using type				= typename stack::type;
		using const_type		= typename stack::const_type;
		using iterator			= typename stack::iterator;
		using const_iterator	= typename stack::const_iterator;

		using stack::dynamic_stack;
		protected:
			//TODO: second array_to_array can be optimised by b_iterator
			alloc_inst _provide_spaced_buffer(const iterator& gap_pointer, size_type gap_size) const {
				alloc_inst new_allocator(get_allocated() + gap_size);
				//TODO: size can be negative
				size_type first_section_size = gap_pointer - begin();
				array_to_array(
					begin(),
					begin() + first_section_size,
					new_allocator.operator iterator()
				);
				array_to_array(
					begin() + first_section_size,
					end(),
					new_allocator.operator iterator() + first_section_size + gap_size
				);
				return new_allocator;
			}
		public:
			template<typename U, template<typename> typename It>
			void insert(const It<type>& pos, const U& value) 
			requires is_iteroid_v<It, type> {
				if (pos < begin() || pos > end()) {
					throw out_of_range();
				}
				if (get_used() == get_allocated()) {
					_allocated = _extend_by_el();
					alloc_inst new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(new_allocator.operator iterator() + (pos - begin()), value);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					_allocator = std::move(new_allocator);
				}
				else {
					iterator over_pos = end();
					place_at(over_pos, value);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++_used;
			}
			template<typename U, template<typename> typename It>
			void insert(const It<type>& pos, U&& value) 
			requires is_iteroid_v<It, type> {
				if (pos < begin() || pos > end()) {
					throw out_of_range();
				}
				if (get_used() == get_allocated()) {
					_allocated = _extend_by_el();
					alloc_inst new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(new_allocator.operator iterator() + (pos - begin()), std::move(value));
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					_allocator = std::move(new_allocator);
				}
				else {
					iterator over_pos = end();
					place_at(over_pos, std::move(value));
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++_used;
			}
			
			//TODO: check, if other iterators (pos) can be used
			template<typename U>
			void insert(iterator pos, const U& value, size_type num) {
				if (pos < begin() || pos > end()) {
					throw out_of_range();
				}
				if (get_used() + num > get_allocated()) {
					_allocated = get_used() + num;
					alloc_inst new_allocator = _provide_spaced_buffer(pos, num);
					iterator target = new_allocator.operator iterator() + (pos - begin());
					while (num) {
						place_at(target, value);
						--num;
						++target;
					}
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					_allocator = std::move(new_allocator);
					_used = get_allocated();
				}
				else {
					iterator over_pos = end() + num;
					while (end() != over_pos) {
						place_at(--over_pos, value);
					}
					while (num) {
						over_pos = end();
						while (over_pos != pos) {
							std::swap(*--over_pos, *over_pos);
						}
						++pos;
						++_used;
						--num;
					}
				}
			}
			//TODO: check, if other iterators (pos) can be used
			template<typename U, template<typename> typename ItT, template<typename> typename ItU>
			void insert(iterator pos, const ItT<U>& first, ItU<U> last) 
			requires is_iteroid_v<ItT, U> && is_iteroid_v<ItU, U> {
				if (pos < begin() || pos > end()) {
					throw out_of_range();
				}
				//TODO: check if num can be negative
				size_type num = last - first;
				if (get_used() + num > get_allocated()) {
					_allocated = get_used() + num;
					alloc_inst new_allocator = _provide_spaced_buffer(pos, num);
					iterator target = new_allocator.operator iterator() + (pos - begin());
					array_to_array(
						first,
						last,
						target
					);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					_allocator = std::move(new_allocator);
					_used = get_allocated();
				}
				else {
					iterator over_pos = end() + num;
					while (end() != over_pos) {
						place_at(--over_pos, *--last);
					}
					while (num) {
						over_pos = end();
						while (over_pos != pos) {
							std::swap(*--over_pos, *over_pos);
						}
						++pos;
						++_used;
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
				if (pos < begin() || pos > end()) {
					throw out_of_range();
				}
				if (get_used() == get_allocated()) {
					_allocated = _extend_by_el();
					alloc_inst new_allocator = _provide_spaced_buffer(pos, 1);
					place_at(new_allocator.operator iterator() + (pos - begin()), std::forward<Args>(args)...);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					_allocator = std::move(new_allocator);
				}
				else {
					iterator over_pos = end();
					place_at(over_pos, std::forward<Args>(args)...);
					while (over_pos != pos) {
						std::swap(*--over_pos, *over_pos);
					}
				}
				++_used;
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
					if (pos < begin() || pos > end()) {
						throw out_of_range();
					}
				}
				--_used;
				while (pos != end()) {
					std::swap(*++pos, *pos);
				}
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destuct_at(end());
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
					if (first < begin() || last > end()) {
						throw out_of_range();
					}
				}
				size_type size = last - first;
				while (last != end()) {
					std::swap(*first, *last);
					++first;
					++last;
				}
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(end() - size, end());
				}
				_used -= size;
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
					if (pos < begin() || pos > end()) {
						throw out_of_range();
					}
				}
				--_used;
				std::swap(*pos, *(end()));
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destuct_at(end());
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
					if (first < begin() || last > end()) {
						throw out_of_range();
					}
				}
				if (last == end()) {
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(first, end());
					}
					_used -= last - first;
				}
				else {
					iterator over_pos = end();
					_used -= last - first;
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
