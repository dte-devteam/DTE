#pragma once
#include "memory/memory.hpp"
namespace dte_utils {
	template<typename T>
	struct unique_ref : pointer_base<T> {
		using type		= typename pointer_base<T>::type;
		using pointer	= typename pointer_base<T>::pointer;
		public:
			unique_ref(pointer instance = nullptr) noexcept : pointer_base<T>(instance) {}
			template<typename U>
			unique_ref(const raw_pointer<U>& instance) noexcept 
			requires(std::is_convertible_v<typename raw_pointer<U>::pointer, pointer>) : unique_ref(instance.operator raw_pointer<U>::pointer()) {}
			unique_ref(const unique_ref&) = delete;
			unique_ref(unique_ref&& other) noexcept : pointer_base<T>(other._instance) {
				other._instance = nullptr;
			}
			~unique_ref() {
				cdelete(remove_const_ptr_base(*this));
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