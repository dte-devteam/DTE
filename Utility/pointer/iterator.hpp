#pragma once
#include "pointer_base.hpp"
namespace dte_utils {
	template<template<typename> typename It, typename T>
	inline constexpr bool is_iteroid_v = true; //TODO
	template<typename T>
	using iterable_pointer = std::conditional_t<is_field_v<drop_pointer_t<T>>, T*, T>;
	template<typename T>
	using f_iterator = pointer_base<iterable_pointer<T>>;
	template<typename T>
	struct b_iterator : pointer_base<iterable_pointer<T>> {
		using raw_type	= typename pointer_base<iterable_pointer<T>>::raw_type;
		using type		= typename pointer_base<iterable_pointer<T>>::type;
		using pointer	= typename pointer_base<iterable_pointer<T>>::pointer;
		using pointer_base<iterable_pointer<T>>::pointer_base;
		public:
			constexpr b_iterator operator+(ptrdiff_t add) const noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance - add };
			}
			constexpr b_iterator& operator+=(ptrdiff_t add) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				_instance -= add;
				return *this;
			}

			constexpr b_iterator operator-(ptrdiff_t sub) const noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance + sub };
			}
			constexpr b_iterator& operator-=(ptrdiff_t sub) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				_instance += sub;
				return *this;
			}
			constexpr ptrdiff_t operator-(pointer ptr) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return _instance - ptr;
			}
			constexpr ptrdiff_t operator-(const b_iterator& other) const noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return _instance - other._instance;
			}

			constexpr b_iterator& operator++() noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				--_instance;
				return *this;
			}
			constexpr b_iterator operator++(int) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance-- };
			}

			constexpr b_iterator& operator--() noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				++_instance;
				return *this;
			}
			constexpr b_iterator operator--(int) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance++ };
			}
	};
}