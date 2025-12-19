#pragma once
#include "mem_handler.hpp"
#include "pointer/iterator.hpp"
#include <utility>
namespace dte_utils {
	//TODO: is_allocator
	template<typename T>
	struct allocator : mem_handler {
		using size_type			= mem_handler::size_type;
		using type				= T;
		using const_type		= const type;
		using iterator			= f_iterator<type>;
		using const_iterator	= f_iterator<const_type>;
		public:
			allocator(size_type num = 0) : mem_handler(num * sizeof(type), alignof(type)) {}
			allocator(const allocator&) = delete;
			allocator(allocator&& other) noexcept : mem_handler(std::move(other)) {}
			allocator& operator=(const allocator&) = delete;
			allocator& operator=(allocator&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_ptr, other._ptr);
				return *this;
			}
			operator iterator() noexcept {
				return static_cast<type*>(_ptr);
			}
			operator const_iterator() const noexcept {
				return static_cast<const_type*>(_ptr);
			}
	};
}