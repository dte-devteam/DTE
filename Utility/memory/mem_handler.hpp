#pragma once
#include "memory.hpp"
namespace dte_utils {
	struct mem_handler {
		using size_type = size_t;
		using type = void;
		using const_type = const type;
		using pointer = type*;
		using const_pointer = const_type*;
		protected:
			pointer _ptr;
		public:
			mem_handler(size_type size = 0) : _ptr(xmalloc(size)) {}
			mem_handler(const mem_handler&) = delete;
			mem_handler(mem_handler&& other) noexcept : _ptr(other) {
				other._ptr = nullptr;
			}
			~mem_handler() {
				free(_ptr);
			}
			void resize(size_type size) {
				_ptr = xrealloc(_ptr, size);
			}
			mem_handler& operator=(const mem_handler&) = delete;
			mem_handler& operator=(mem_handler&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_ptr, other._ptr);
				return *this;
			}
			operator pointer() {
				return _ptr;
			}
			operator const_pointer() const {
				return _ptr;
			}
	};
}