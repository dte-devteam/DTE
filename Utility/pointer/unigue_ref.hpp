#pragma once
#include "memory/memory.hpp"
#include "pointer_base.hpp"
namespace dte_utils {
	template<typename T>
	struct unique_ref : pointer_base<T> {
		using type = pointer_base<T>::type;
		using pointer = pointer_base<T>::pointer;
		using const_pointer = pointer_base<T>::const_pointer;
		public:
			unique_ref(const_pointer instance = nullptr) noexcept : pointer_base<T>(instance) {}
			unique_ref(const unique_ref&) = delete;
			unique_ref(unique_ref&& other) noexcept : pointer_base<T>(other._instance) {
				other._instance = nullptr;
			}
			~unique_ref() {
				cdelete(_instance);
			}

			unique_ref& operator=(const unique_ref&) = delete;
			unique_ref& operator=(unique_ref&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_instance, other._instance);
				return *this;
			}
	};
}