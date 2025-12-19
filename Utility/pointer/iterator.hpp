#pragma once
#include "pointer_base.hpp"
namespace dte_utils {
	template<template<typename> typename It, typename T>
	inline constexpr bool iteroid_v = std::is_base_of_v<iterator_base<T>, It<T>>&& requires(const It<T>& ct, It<T>& t) {
		++t; --t;
		t++; t--;
	};
	template<typename T>
	struct iterator_base : pointer_base<T> {
		using size_type = ptrdiff_t;
		using type = pointer_base<T>::type;
		using pointer = pointer_base<T>::pointer;
		using pointer_base<T>::pointer_base;
		protected:
			void _inc(size_type add = 1) noexcept {
				if constexpr (std::is_void_v<type>) {
					this->_instance = static_cast<char*>(this->_instance) + add;
				}
				else {
					this->_instance += add;
				}
			}
			void _dec(size_type sub = 1) noexcept {
				if constexpr (std::is_void_v<type>) {
					this->_instance = static_cast<char*>(this->_instance) - sub;
				}
				else {
					this->_instance -= sub;
				}
			}
		public:
			iterator_base(const pointer_base<T>& other) noexcept : pointer_base<T>(other) {}
	};
	template<typename T>
	struct f_iterator : iterator_base<T> {
		using size_type = iterator_base<T>::size_type;
		using type = iterator_base<T>::type;
		using pointer = iterator_base<T>::pointer;
		using iterator_base<T>::iterator_base;
		public:
			f_iterator& operator++() noexcept {
				this->_inc();
				return *this;
			}
			f_iterator operator++(int) noexcept {
				f_iterator iter(*this);
				this->_inc();
				return iter;
			}
			f_iterator& operator--() noexcept {
				this->_dec();
				return *this;
			}
			f_iterator operator--(int) noexcept {
				f_iterator iter(*this);
				this->_dec();
				return iter;
			}
			f_iterator& operator+=(size_type add) noexcept {
				this->_inc(add);
				return *this;
			}
			f_iterator& operator-=(size_type sub) noexcept {
				this->_dec(sub);
				return *this;
			}
			f_iterator operator+(size_type add) const noexcept {
				f_iterator iter(*this);
				iter._inc(add);
				return iter;
			}
			f_iterator operator-(size_type sub) const noexcept {
				f_iterator iter(*this);
				iter._dec(sub);
				return iter;
			}
			template<template<typename> typename It>
			size_type operator-(const It<type>& other) const noexcept 
			requires iteroid_v<It, type> {
				return this->_instance - other.operator type*();
			}
			template<template<typename> typename It>
			size_type operator-(const It<const type>& other) const noexcept
			requires iteroid_v<It, const type> {
				return this->_instance - other.operator const type*();
			}
			template<bool is_fail_safe = false>
			type& operator[](size_type index) const noexcept(is_fail_safe) {
				return (*this + index).operator*<is_fail_safe>();
			}
			template<template<typename> typename It>
			bool operator>(const It<type>& other) const noexcept
			requires iteroid_v<It, type> {
				return *this - other > 0;
			}
			template<template<typename> typename It>
			bool operator<(const It<type>& other) const noexcept
			requires iteroid_v<It, type> {
				return *this - other < 0;
			}
			template<template<typename> typename It>
			bool operator>=(const It<type>& other) const noexcept
			requires iteroid_v<It, type> {
				return !(*this < other);
			}
			template<template<typename> typename It>
			bool operator<=(const It<type>& other) const noexcept
				requires iteroid_v<It, type> {
				return !(*this > other);
			}
	};
	template<typename T>
	struct b_iterator : iterator_base<T> {
		using size_type = iterator_base<T>::size_type;
		using type = iterator_base<T>::type;
		using pointer = iterator_base<T>::pointer;
		using iterator_base<T>::iterator_base;
		public:
			b_iterator& operator++() noexcept {
				this->_dec();
				return *this;
			}
			b_iterator operator++(int) noexcept {
				b_iterator iter(*this);
				this->_dec();
				return iter;
			}
			b_iterator& operator--() noexcept {
				this->_inc();
				return *this;
			}
			b_iterator operator--(int) noexcept {
				b_iterator iter(*this);
				this->_inc();
				return iter;
			}
			b_iterator& operator+=(size_type sub) noexcept {
				this->_dec(sub);
				return *this;
			}
			b_iterator& operator-=(size_type add) noexcept {
				this->_inc(add);
				return *this;
			}
			b_iterator operator+(size_type sub) const noexcept {
				b_iterator iter(*this);
				iter._dec(sub);
				return iter;
			}
			b_iterator operator-(size_type add) const noexcept {
				b_iterator iter(*this);
				iter._inc(add);
				return iter;
			}
			template<template<typename> typename It>
			size_type operator-(const It<type>& other) const noexcept
			requires iteroid_v<It, type> {
				return this->_instance - other._instance;
			}
			template<bool is_fail_safe = false>
			type& operator[](size_type index) const noexcept(is_fail_safe) {
				return (*this + index).operator*<is_fail_safe>();
			}
			template<template<typename> typename It>
			bool operator>(const It<type>& other) const noexcept
				requires iteroid_v<It, type> {
				return *this - other > 0;
			}
			template<template<typename> typename It>
			bool operator<(const It<type>& other) const noexcept
				requires iteroid_v<It, type> {
				return *this - other < 0;
			}
			template<template<typename> typename It>
			bool operator>=(const It<type>& other) const noexcept
				requires iteroid_v<It, type> {
				return !(*this < other);
			}
			template<template<typename> typename It>
			bool operator<=(const It<type>& other) const noexcept
				requires iteroid_v<It, type> {
				return !(*this > other);
			}
	};
}