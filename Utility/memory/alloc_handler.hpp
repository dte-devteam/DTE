#pragma once
#include "allocator.hpp"
namespace dte_utils {
	template<typename T, template<typename> typename A = allocator>
	requires is_allocator_v<A, T>
	struct alloc_handler {
		using size_type = A<T>::size_type;
		using type = A<T>::type;
		using const_type = A<T>::const_type;
		using pointer = A<T>::pointer;
		using const_pointer = A<T>::const_pointer;
		protected:
			size_type _allocated;
			A<T> _allocator;
		public:
			alloc_handler(size_type num = 0) : _allocated(num), _allocator(num) {}
			alloc_handler(const alloc_handler& other) : alloc_handler(other.get_allocated()) {}
			alloc_handler(alloc_handler&& other) noexcept : _allocated(other.get_allocated()), _allocator(std::move(other._allocator)) {
				other._allocated = 0;
			}
			template<typename U, template<typename> typename A = allocator>
			alloc_handler(const alloc_handler<U, A>& other) : alloc_handler(other.get_allocated()) {}
			size_type get_allocated() const {
				return _allocated;
			}
			alloc_handler& operator=(const alloc_handler&) = delete;
			alloc_handler& operator=(alloc_handler&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_allocated, other._allocated);
				std::swap(_allocator, other._allocator);
				return *this;
			}
	};
}