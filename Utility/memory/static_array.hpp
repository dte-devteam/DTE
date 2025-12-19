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
		using iterator = f_iterator<type>;
		using const_iterator = f_iterator<const_type>;
		alignas(type) char arr[sizeof(type) * N];
		static_array() noexcept(std::is_nothrow_default_constructible_v<type>)
		requires std::is_default_constructible_v<type> {
			iterator iter = begin();
			while (iter != end()) {
				place_at(iter);
				++iter;
			}
		}
		template<typename U>
		requires std::is_constructible_v<type, const U&>
		static_array(const U& instance) noexcept(std::is_nothrow_constructible_v<type, const U&>) {
			iterator iter = begin();
			while (iter != end()) {
				place_at(iter, instance);
				++iter;
			}
		}
		template<typename U>
		requires std::is_constructible_v<type, const U&>
		static_array(const U (&arr)[N]) noexcept(std::is_nothrow_constructible_v<type, const U&>) {
			iterator iter = begin();
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
			iterator iter = begin();
			for (const_type& val : il) {
				place_at(iter, val);
				++iter;
			}
		}
		static_array(const static_array& other) noexcept(std::is_nothrow_copy_constructible_v<type>)
		requires std::is_copy_constructible_v<type> {
			iterator iter = begin();
			for (const_type& val : other) {
				place_at(iter, val);
				++iter;
			}
		}
		template<typename U>
		requires std::is_constructible_v<type, const U&>
		static_array(const static_array<U, N> other) noexcept(std::is_nothrow_constructible_v<type, const U&>) {
			iterator iter = begin();
			for (const U& val : other) {
				place_at(iter, val);
				++iter;
			}
		}


		iterator begin() noexcept {
			return reinterpret_cast<type*>(arr);
		}
		const_iterator begin() const noexcept {
			return reinterpret_cast<const_type*>(arr);
		}
		iterator end() noexcept {
			return begin() + N;
		}
		const_iterator end() const noexcept {
			return begin() + N;
		}


		static_array& operator=(const static_array& other) noexcept(std::is_nothrow_copy_assignable_v<type>)
		requires std::is_copy_assignable_v<type> {
			if (this == &other) {
				return *this;
			}
			iterator iter = begin();
			for (const_type& val : other) {
				*iter = val;
				++iter;
			}
			return *this;
		}
		template<typename U>
		requires std::is_assignable_v<type, const U&>
		static_array& operator=(const static_array<U, N>& other) noexcept(std::is_nothrow_assignable_v<type, const U&>) {
			iterator iter = begin();
			for (const U& val : other) {
				*iter = val;
				++iter;
			}
			return *this;
		}
		static_array& operator=(static_array&& other) noexcept(std::is_nothrow_swappable_v<type>)
		requires std::is_swappable_v<type> {
			if (this == &other) {
				return *this;
			}
			iterator iter = begin();
			for (type& val : other) {
				std::swap(*iter, val);
				++iter;
			}
			return *this;
		}

		template<bool is_fail_safe = false>
		type& operator[](size_t index) noexcept(is_fail_safe) {
			if constexpr (!is_fail_safe) {
				if (index < N) {
					return begin()[index];
				}
				throw out_of_range();
			}
			else {
				return begin()[index];
			}
		}
		template<bool is_fail_safe = false>
		const_type& operator[](size_t index) const noexcept(is_fail_safe) {
			if constexpr (!is_fail_safe) {
				if (index < N) {
					return begin()[index];
				}
				throw out_of_range();
			}
			else {
				return begin()[index];
			}
		}
	};
}