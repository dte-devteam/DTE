#pragma once
#include "mem_handler.hpp"
namespace dte_utils {
	template<template<typename> typename U, typename T>
	concept is_allocator = sizeof_limits<T> && requires(U<T> u) {
		typename U<T>::size_type;
		typename U<T>::type;
		typename U<T>::const_type;

		static_cast<typename U<T>::pointer>(u);
		static_cast<typename U<T>::const_pointer>(u);
	};

	template<sizeof_limits T>
	struct allocator : mem_handler {
		using size_type = mem_handler::size_type;
		using type = T;
		using const_type = const type;
		using pointer = type*;
		using const_pointer = const_type*;
		public:
			allocator(size_type num = 0) : mem_handler(num * sizeof(T)) {}
			allocator(const allocator&) = delete;
			allocator(allocator&& other) noexcept : mem_handler(std::move(other)) {}
			void resize(size_type num) {
				static_assert(std::is_trivial_v<T>, "do not try reallocating nontrivial data");
				mem_handler::resize(num * sizeof(T));
			}
			allocator& operator=(const allocator&) = delete;
			allocator& operator=(allocator&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_ptr, other._ptr);
				return *this;
			}
			operator pointer() {
				return static_cast<pointer>(_ptr);
			}
			operator const_pointer() const {
				return static_cast<const_pointer>(_ptr);
			}
	};
}