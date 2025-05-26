#pragma once
#include "mem_handler.hpp"
#include "types.hpp"
namespace dte_utils {
	struct xmem_wrapper : mem_handler {
		protected:
			destructor _destructor;
			void kill_value() {
				if (_destructor && _ptr) {
					_destructor(_ptr);
				}
			}
		public:
			xmem_wrapper(pointer ptr = nullptr, destructor destr_func = nullptr) : _destructor(destr_func), mem_handler(ptr) {}
			~xmem_wrapper() {
				kill_value();
			}

			xmem_wrapper(const xmem_wrapper&) = delete;
			xmem_wrapper(xmem_wrapper&& other) noexcept : _destructor(other._destructor), mem_handler(std::move(other)) {}

			destructor get_destructor() {
				return _destructor;
			}
			void set_new_value(pointer ptr = nullptr, destructor destr_func = nullptr) {
				if (_ptr == ptr) {
					return;
				}
				kill_value();
				_ptr = ptr;
				_destructor = destr_func;
			}

			xmem_wrapper& operator=(const xmem_wrapper&) = delete;
			xmem_wrapper& operator=(xmem_wrapper&& other) noexcept {
				mem_handler::operator=(std::move(other));
				std::swap(_destructor, other._destructor);
				return *this;
			}
	};
}