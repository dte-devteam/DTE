#pragma once
#include "pointer/raw_pointer.hpp"
#include "function_traits.hpp"
#include "exceptions/memory_exception.hpp"
namespace dte_utils {
	template<typename T>
	struct pointer_base : raw_pointer<T> {
		using type		= typename raw_pointer<T>::type;
		using pointer	= typename raw_pointer<T>::pointer;
		using raw_pointer<T>::raw_pointer;
		public:
			template<bool is_fail_safe = false>
			std::add_lvalue_reference_t<type> operator*() const noexcept(is_fail_safe)
			requires(!(return_type_v<type> || std::is_void_v<type>)) {
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

			template<bool is_fail_safe = false, typename ...Args>
			return_type_t<type> operator()(Args&&... args) const 
			noexcept(return_type_noexcept<type> && is_fail_safe)
			requires(return_type_v<type>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return _instance(std::forward<Args>(args)...);
			}
			template<bool is_fail_safe = false, typename ...Args>
			is_functor_t<type, Args...> operator()(Args&&... args) const 
			noexcept(is_functor_noexcept_v<type, Args...> && is_fail_safe) 
			requires(is_functor_v<type, Args...>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return _instance->operator()(std::forward<Args>(args)...);
			}
	};
	template<typename C, typename V>
	struct pointer_base<V(C::*)> : raw_pointer<V(C::*)> {
		using type		= typename raw_pointer<V(C::*)>::type;
		using pointer	= typename raw_pointer<V(C::*)>::pointer;
		using raw_pointer<V(C::*)>::raw_pointer;
		public:
			template<bool is_fail_safe = false, typename ...Args>
			requires(std::is_same_v<args_list<Args...>, field_function_a<pointer>>)
			std::conditional_t<is_field_function_v<pointer>, field_function_r<pointer>, std::add_lvalue_reference_t<V>> operator()(C& c, Args&&... args) const 
			noexcept(is_fail_safe && (is_field_function_v<pointer> ? is_field_function_noexcept_v<pointer> : true)) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				if constexpr (is_field_function_v<pointer>) {
					return (c.*_instance)(std::forward<Args>(args)...);
				}
				else {
					return c.*_instance;
				}
			}
			template<bool is_fail_safe = false, typename ...Args>
			requires(
				std::is_same_v<args_list<Args...>, field_function_a<pointer>> &&
				(is_field_function_v<pointer> ? is_field_function_const_v<pointer> : true)
			)
			std::conditional_t<is_field_function_v<pointer>, field_function_r<pointer>, std::add_lvalue_reference_t<std::add_const_t<V>>> operator()(const C& c, Args&&... args) const 
			noexcept(is_fail_safe && (is_field_function_v<pointer> ? is_field_function_noexcept_v<pointer> : true)) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				if constexpr (is_field_function_v<pointer>) {
					return (c.*_instance)(std::forward<Args>(args)...);
				}
				else {
					return c.*_instance;
				}
			}
	};
	template<typename T>
	inline pointer_base<T> remove_const_ptr_base(const pointer_base<T>& ptr) noexcept {
		return ptr;
	}
	template<typename T>
	inline pointer_base<T> remove_const_ptr_base(const pointer_base<const T>& ptr) noexcept {
		return pointer_base<T>(remove_const_ptr(ptr.operator->()));
	}
	template<typename T, typename F>
	inline pointer_base<T> static_cast_ptr_base(const pointer_base<F>& ptr) noexcept
	requires(is_static_castable_v<typename pointer_base<F>::pointer, typename pointer_base<T>::pointer>) {
		return pointer_base<T>(static_cast<pointer_base<T>::pointer>(ptr.operator->()));
	}
}