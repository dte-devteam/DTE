#pragma once
#include "memory/memory.hpp"
#include "ref.hpp"
namespace dte_utils {
	template<allocatable T>
	struct unique_ref {
		protected:
			ref_pointer<T> _instance;
		public:
			unique_ref(ref_pointer<T> instance = nullptr) : _instance(instance) {}	//TODO: temp type
			unique_ref(const unique_ref&) = delete;
			unique_ref(unique_ref&& other) noexcept : _instance(other.instance) {
				other._instance = nullptr;
			}
			~unique_ref() {
				if (_instance) {
					if constexpr (!std::is_trivially_destructible_v<T>) {
						destuct_at(_instance);
					}
					if constexpr (!return_type_v<T>) {
						free(_instance);
					}
				}
			}

			unique_ref& operator=(const unique_ref&) = delete;
			unique_ref& operator=(unique_ref&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_instance, other._instance);
				return *this;
			}

			T& operator*() const 
			requires !return_type_v<T> {
				if (!_instance) {
					throw nullptr_access();
				}
				return *_instance;
			}
			T* operator->() const 
			requires !return_type_v<T> {
				if (!_instance) {
					throw nullptr_access();
				}
				return _instance;
			}
			template<typename R = return_type_t<T>, typename ...Args>
			R operator()(Args&&... args) const {
				if (!_instance) {
					throw nullptr_access();
				}
				return _instance(std::forward<Args>(args)...);
			}
	};
}