#pragma once
namespace dte_utils {
	struct mem_handler {
		using size_type		= size_t;
		using type			= void;
		using const_type	= typename const type;
		using pointer		= typename type*;
		using const_pointer	= typename const_type*;
		protected:
			pointer _ptr;
		public:
			mem_handler(size_type size = 0, size_type alignment = 1);
			mem_handler(const mem_handler&) = delete;
			mem_handler(mem_handler&& other) noexcept;
			~mem_handler();
			mem_handler& operator=(const mem_handler&) = delete;
			mem_handler& operator=(mem_handler&& other) noexcept;
	};
}