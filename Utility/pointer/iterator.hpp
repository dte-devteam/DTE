#pragma once
#include "pointer_base.hpp"
namespace dte_utils {
	template<template<typename> typename It, typename T>
	inline constexpr bool is_iteroid_v = std::is_base_of_v<pointer_base<T>, It<T>> && requires(const It<T>& ct, It<T>& t) {
		{ ++t } noexcept -> std::same_as< It<T>&>;
		{ --t } noexcept -> std::same_as< It<T>&>;
		{ t++ } noexcept -> std::same_as< It<T>>;
		{ t-- } noexcept -> std::same_as< It<T>>;

		{ t += std::declval<typename It<T>::size_type>() } noexcept -> std::same_as<It<T>&>;
		{ t -= std::declval<typename It<T>::size_type>() } noexcept -> std::same_as<It<T>&>;

		{ ct + std::declval<typename It<T>::size_type>() } noexcept -> std::same_as<It<T>>;
		{ ct - std::declval<typename It<T>::size_type>() } noexcept -> std::same_as<It<T>>;

		{ ct - ct } noexcept -> std::same_as<typename It<T>::size_type>;

		//we ingnore [] operator, because it equals:
		//(*this + index).operator*<is_fail_safe>()
		//*this + index is described above
		//operator*<is_fail_safe> is described in pointer_base
		//also void specialisation forbids * & [] operators

		{ ct > ct } noexcept -> std::same_as<bool>;
		{ ct < ct } noexcept -> std::same_as<bool>;
		{ ct >= ct } noexcept -> std::same_as<bool>;
		{ ct <= ct } noexcept -> std::same_as<bool>;
	};

	template<typename T>
	struct iterator_base : pointer_base<T> {
		using size_type	= typename ptrdiff_t;
		using type		= typename pointer_base<T>::type;
		using pointer	= typename pointer_base<T>::pointer;
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
		using size_type	= typename iterator_base<T>::size_type;
		using type		= typename iterator_base<T>::type;
		using pointer	= typename iterator_base<T>::pointer;
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
			template<typename U>
			size_type operator-(const iterator_base<U>& other) const noexcept 
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				if constexpr (std::is_void_v<type>) {
					return static_cast<char*>(this->_instance) - static_cast<char*>(other.operator iterator_base<U>::pointer());
				}
				else {
					return this->_instance - other.operator iterator_base<U>::pointer();
				}
			}
			template<bool is_fail_safe = false>
			std::add_lvalue_reference_t<type> operator[](size_type index) const noexcept(is_fail_safe)
			requires !std::is_void_v<type> {
				return (*this + index).operator*<is_fail_safe>();
			}
			template<typename U>
			bool operator>(const iterator_base<U>& other) const noexcept 
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return (*this - other) > 0;
			}
			template<typename U>
			bool operator<(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return (*this - other) < 0;
			}
			template<typename U>
			bool operator>=(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return !(*this < other);
			}
			template<typename U>
			bool operator<=(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return !(*this > other);
			}
	};
	template<typename T>
	struct b_iterator : iterator_base<T> {
		using size_type	= typename iterator_base<T>::size_type;
		using type		= typename iterator_base<T>::type;
		using pointer	= typename iterator_base<T>::pointer;
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
			template<typename U>
			size_type operator-(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				if constexpr (std::is_void_v<type>) {
					return static_cast<char*>(this->_instance) - static_cast<char*>(other.operator iterator_base<U>::pointer());
				}
				else {
					return this->_instance - other.operator iterator_base<U>::pointer();
				}
			}
			template<bool is_fail_safe = false>
			std::add_lvalue_reference_t<type> operator[](size_type index) const noexcept(is_fail_safe)
			requires !std::is_void_v<type> {
				return (*this + index).operator*<is_fail_safe>();
			}
			template<typename U>
			bool operator>(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return (*this - other) > 0;
			}
			template<typename U>
			bool operator<(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return (*this - other) < 0;
			}
			template<typename U>
			bool operator>=(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return !(*this < other);
			}
			template<typename U>
			bool operator<=(const iterator_base<U>& other) const noexcept
			requires std::is_same_v<std::remove_cv_t<typename iterator_base<U>::type>, std::remove_cv_t<type>> {
				return !(*this > other);
			}
	};
}