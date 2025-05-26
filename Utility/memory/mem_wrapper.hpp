#pragma once
#include "mem_handler.hpp"
#include "memory.hpp"
#include "types.hpp"
namespace dte_utils {
	template<typename T>
	struct tmem_wrapper : mem_handler {
		using type = T;
		using const_type = const type;
		tmem_wrapper() : mem_handler(sizeof(type)) {
			place_at(static_cast<type*>(_ptr));
		}
		template<typename U>
		tmem_wrapper(const U& value) : mem_handler(sizeof(type)) {
			place_at(static_cast<type*>(_ptr), value);
		}
		template<typename U>
		tmem_wrapper(U&& value) : mem_handler(sizeof(type)) {
			place_at(static_cast<type*>(_ptr), std::move(value));
		}
		~tmem_wrapper() {
			if constexpr (!std::is_trivially_destructible_v<type>) {
				if (_ptr) {
					destuct_at(static_cast<type*>(_ptr));
				}
			}
		}

		tmem_wrapper(const tmem_wrapper&) = delete;
		tmem_wrapper(tmem_wrapper&& other) noexcept : mem_handler(other) {}

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

		tmem_wrapper& operator=(const tmem_wrapper&) = delete;
		tmem_wrapper& operator=(tmem_wrapper&& other) noexcept {
			mem_handler::operator=(std::move(other));
			return *this;
		}
	};
}