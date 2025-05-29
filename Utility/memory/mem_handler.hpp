#pragma once
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
			mem_handler(size_type size = 0);
			mem_handler(pointer ptr);
			mem_handler(const mem_handler&) = delete;
			mem_handler(mem_handler&& other) noexcept;
			~mem_handler();
			void resize(size_type size);
			mem_handler& operator=(pointer ptr);
			mem_handler& operator=(const mem_handler&) = delete;
			mem_handler& operator=(mem_handler&& other) noexcept;
			operator pointer();
			operator const_pointer() const;
	};
}