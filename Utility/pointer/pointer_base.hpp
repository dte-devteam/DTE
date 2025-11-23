#pragma once
#include "function_traits.hpp"
#include "exceptions/memory_exception.hpp"
namespace dte_utils {
	template<typename T>
	struct pointer_base {
		using type = T;
		using pointer = type*;
		protected:
			pointer _instance;
		public:
			pointer_base(pointer instance) noexcept : _instance(instance) {}

			template<bool is_fail_safe = false>
			type& operator*() const noexcept(is_fail_safe)
			requires !return_type_v<type> {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return *_instance;
			}
			pointer operator->() const noexcept {
				return _instance;
			}
			operator pointer() const noexcept {
				return this->_instance;
			}

			ptrdiff_t operator-(const pointer_base& other) const noexcept {
				return _instance - other._instance;
			}
			bool operator==(const pointer_base& other) const noexcept {
				return _instance == other._instance;
			}
			bool operator!=(const pointer_base& other) const noexcept {
				return !(*this == other);
			}

			template<bool is_fail_safe = false, typename ...Args>
			requires return_type_v<type>
			return_type_t<type> operator()(Args&&... args) const 
			noexcept(return_type_noexcept<type> && is_fail_safe) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return _instance(std::forward<Args>(args)...);
			}
			template<bool is_fail_safe = false, typename ...Args>
			requires is_functor_v<type, Args...>
			is_functor_t<type, Args...> operator()(Args&&... args) const 
			noexcept(is_functor_noexcept_v<type, Args...> && is_fail_safe) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return _instance->operator()(std::forward<Args>(args)...);
			}
	};
	template<>
	struct pointer_base<void> {
		using type = void;
		using pointer = type*;
		protected:
			pointer _instance;
		public:
			pointer_base(pointer instance) noexcept : _instance(instance) {}
	};
}