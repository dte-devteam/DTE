#pragma once
#include "memory/memory.hpp"
#include "ref.hpp"
namespace dte_utils {
	template<typename T>
	struct unique_ref {
		using type = T;
		protected:
			ref_pointer<type> _instance;
		public:
			unique_ref(ref_pointer<type> instance = nullptr) : _instance(instance) {}
			unique_ref(const unique_ref&) = delete;
			unique_ref(unique_ref&& other) noexcept : _instance(other.instance) {
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

			type& operator*() const 
			requires !return_type_v<type> {
				if (!_instance) {
					throw nullptr_access();
				}
				return *_instance;
			}
			type* operator->() const 
			requires !return_type_v<type> {
				if (!_instance) {
					throw nullptr_access();
				}
				return _instance;
			}
			template<typename R = return_type_t<type>, typename ...Args>
			R operator()(Args&&... args) const {
				if (!_instance) {
					throw nullptr_access();
				}
				return _instance(std::forward<Args>(args)...);
			}
	};
}