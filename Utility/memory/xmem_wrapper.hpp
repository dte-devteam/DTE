#pragma once
#include "mem_handler.hpp"
#include "types.hpp"
namespace dte_utils {
	struct xmem_wrapper : mem_handler {
		using pointer = mem_handler::pointer;
		protected:
			destructor* _destructor;
			void kill_value();
		public:
			xmem_wrapper(pointer ptr = nullptr, destructor* destr_func = nullptr);
			~xmem_wrapper();

			xmem_wrapper(const xmem_wrapper&) = delete;
			xmem_wrapper(xmem_wrapper&& other) noexcept;

			destructor* get_destructor();
			void set_new_value(pointer ptr = nullptr, destructor* destr_func = nullptr);

			xmem_wrapper& operator=(const xmem_wrapper&) = delete;
			xmem_wrapper& operator=(xmem_wrapper&& other) noexcept;
	};
}