#pragma once
#include "type_properties.hpp"
#include "function_traits.hpp"
#include "exceptions/memory_exception.hpp"
namespace dte_utils {
	template<typename T>
	struct pointer_base {
		using type		= typename T;
		using pointer	= typename type*;
		protected:
			pointer _instance;
		public:
			pointer_base(pointer instance = nullptr) noexcept : _instance(instance) {}
			template<typename U>
			pointer_base(const pointer_base<U>& other) noexcept 
			requires std::is_convertible_v<typename pointer_base<U>::pointer, pointer> : _instance(other.operator->()) {}
			
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
			//just for "for loop", operator->() is much shorter than "operator pointer_base<T>::pointer()"
			explicit operator pointer() const noexcept {
				return this->_instance;
			}

			template<typename U>
			bool operator==(U* ptr) const noexcept
			requires has_equal_to_operator_v<pointer, U*> {
				return _instance == ptr;
			}
			template<typename U>
			bool operator!=(U* ptr) const noexcept
			requires has_equal_to_operator_v<pointer, U*> {
				return !(*this == ptr);
			}

			template<typename U>
			bool operator==(const pointer_base<U>& other) const noexcept
			requires has_equal_to_operator_v<pointer, typename pointer_base<U>::pointer> {
				return _instance == other.operator->();
			}
			template<typename U>
			bool operator!=(const pointer_base<U>& other) const noexcept
			requires has_equal_to_operator_v<pointer, typename pointer_base<U>::pointer> {
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
			requires is_functor_v<type, Args...> //no need for void check - void never returns
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
			pointer_base(pointer instance = nullptr) noexcept : _instance(instance) {}


			pointer operator->() const noexcept {
				return _instance;
			}
			//just for "for loop", operator->() is much shorter than "operator pointer_base<T>::pointer()"
			explicit operator pointer() const noexcept {
				return this->_instance;
			}

			template<typename U>
			bool operator==(U* ptr) const noexcept
			requires has_equal_to_operator_v<pointer, U*> {
				return _instance == ptr;
			}
			template<typename U>
			bool operator!=(U* ptr) const noexcept
			requires has_equal_to_operator_v<pointer, U*> {
				return !(*this == ptr);
			}

			template<typename U>
			bool operator==(const pointer_base<U>& other) const noexcept
			requires has_equal_to_operator_v<pointer, typename pointer_base<U>::pointer> {
				return _instance == other.operator->();
			}
			template<typename U>
			bool operator!=(const pointer_base<U>& other) const noexcept
			requires has_equal_to_operator_v<pointer, typename pointer_base<U>::pointer> {
				return !(*this == other);
			}
	};
}