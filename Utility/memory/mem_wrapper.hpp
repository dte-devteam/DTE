#pragma once
#include "mem_handler.hpp"
namespace dte_utils {
	template<allocatable T>
	struct mem_wrapper : mem_handler {
		using type = T;
		using const_type = const type;
		mem_wrapper()
		requires std::is_default_constructible_v<type> :
		mem_handler(sizeof(type)) {
			place_at(static_cast<type*>(_ptr));
		}
		template<copy_constructible<type> U>
		mem_wrapper(const U& value) : mem_handler(sizeof(type)) {
			place_at(static_cast<type*>(_ptr), value);
		}
		template<move_constructible<type> U>
		mem_wrapper(U&& value) : mem_handler(sizeof(type)) {
			place_at(static_cast<type*>(_ptr), std::move(value));
		}
		~mem_wrapper() {
			if (_ptr) {
				static_cast<type*>(_ptr)->~type();
			}
		}

		mem_wrapper(const mem_wrapper&) = delete;
		mem_wrapper(mem_wrapper&& other) noexcept : mem_handler(other) {}

		type& get() {
			if (_ptr) {
				return *static_cast<type*>(_ptr);
			}
			throw nullptr_access();
		}
		const_type& get() const {
			if (_ptr) {
				return *static_cast<type*>(_ptr);
			}
			throw nullptr_access();
		}

		mem_wrapper& operator=(const mem_wrapper&) = delete;
		mem_wrapper& operator=(mem_wrapper&& other) noexcept {
			mem_handler::operator=(std::move(other));
			return *this;
		}
	};
}