#pragma once
#include "allocator.hpp"
namespace dte_utils {
	template<typename T, template<typename> typename A = allocator>
	struct alloc_handler {
		using alloc_inst		= typename A<T>;
		using size_type			= typename alloc_inst::size_type;
		using type				= typename alloc_inst::type;
		using const_type		= typename alloc_inst::const_type;
		using iterator			= typename alloc_inst::iterator;
		using const_iterator	= typename alloc_inst::const_iterator;
		protected:
			size_type _allocated;
			alloc_inst _allocator;
		public:
			alloc_handler(size_type num = 0) : _allocated(num), _allocator(num) {}
			alloc_handler(const alloc_handler& other) : alloc_handler(other.get_allocated()) {}
			alloc_handler(alloc_handler&& other) noexcept : _allocated(other.get_allocated()), _allocator(std::move(other._allocator)) {
				other._allocated = 0;
			}
			size_type get_allocated() const noexcept {
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