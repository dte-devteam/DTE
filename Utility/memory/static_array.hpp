#pragma once
#include "memory.hpp"
#include "exceptions/logic_exception.hpp"
#include <initializer_list>
namespace dte_utils {
	template<typename T, size_t N>
	requires (N > 0)
	struct static_array {
		using type = T;
		using const_type = const type;
		using pointer = type*;
		using const_pointer = const_type*;
		type arr[N];
		static_array() noexcept(std::is_nothrow_default_constructible_v<type>)
		requires std::is_default_constructible_v<type> {
			pointer iter = end();
			while (iter != begin()) {
				place_at(--iter);
			}
		}
		template<typename U>
		requires std::is_constructible_v<type, const U&>
		static_array(const U (&arr)[N]) noexcept(std::is_nothrow_constructible_v<type, const U&>) {
			pointer iter = begin();
			for (const U& val : arr) {
				place_at(iter, val);
				++iter;
			}
		}
		static_array(std::initializer_list<type> il)
		requires std::is_copy_constructible_v<type> {
			if (il.size() != N) {
				throw invalid_range();
			}
			pointer iter = begin();
			for (const_type& val : il) {
				place_at(iter, val);
				++iter;
			}
		}
		static_array(const static_array& other) noexcept(std::is_nothrow_copy_constructible_v<type>)
		requires std::is_copy_constructible_v<type> {
			pointer iter = begin();
			for (const_type& val : other) {
				place_at(iter, val);
				++iter;
			}
		}
		template<typename U>
		requires std::is_constructible_v<type, const U&>
		static_array(const static_array<U, N> other) noexcept(std::is_nothrow_constructible_v<type, const U&>) {
			pointer iter = begin();
			for (const U& val : other) {
				place_at(iter, val);
				++iter;
			}
		}


		pointer begin() noexcept {
			return arr;
		}
		const_pointer begin() const noexcept {
			return arr;
		}
		pointer end() noexcept {
			return arr + N;
		}
		const_pointer end() const noexcept {
			return arr + N;
		}


		static_array& operator=(const static_array& other) noexcept(std::is_nothrow_copy_assignable_v<type>)
		requires std::is_copy_assignable_v<type> {
			if (this == &other) {
				return *this;
			}
			pointer iter = begin();
			for (const_type& val : other) {
				*iter = val;
				++iter;
			}
			return *this;
		}
		template<typename U>
		requires std::is_assignable_v<type, const U&>
		static_array& operator=(const static_array<U, N>& other) noexcept(std::is_nothrow_assignable_v<type, const U&>) {
			if (this == &other) {
				return *this;
			}
			pointer iter = begin();
			for (const U& val : other) {
				*iter = val;
				++iter;
			}
			return *this;
		}
		static_array& operator=(static_array&& other) noexcept
		requires std::is_swappable_v<type> {
			if (this == &other) {
				return *this;
			}
			pointer iter = begin();
			for (type& val : other) {
				std::swap(*iter, val);
				++iter;
			}
			return *this;
		}


		type& operator[](size_t index) {
			if (!(index < N)) {
				throw out_of_range();
			}
			return arr[index];
		}
		const_type& operator[](size_t index) const {
			if (!(index < N)) {
				throw out_of_range();
			}
			return arr[index];
		}
	};
}