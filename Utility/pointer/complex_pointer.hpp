#pragma once
#include "pointer_base.hpp"
namespace dte_utils {
	template<typename T, bool = false>
	struct complex_pointer : pointer_base<T> {
		using raw_type	= typename pointer_base<T>::raw_type;
		using type		= typename pointer_base<T>::type;
		using pointer	= typename pointer_base<T>::pointer;
		using owner		= typename void;
		using pointer_base<T>::pointer_base;
	};
	template<typename C, typename V, bool is_const>
	struct complex_pointer<V(C::*), is_const> : pointer_base<V(C::*)> {
		using raw_type	= typename pointer_base<V(C::*)>::raw_type;
		using type		= typename pointer_base<V(C::*)>::type;
		using pointer	= typename pointer_base<V(C::*)>::pointer;
		using owner		= typename std::conditional_t<is_const, const C, C>;
		protected:
			pointer_base<owner> _class;
		public:
			complex_pointer(const pointer_base<V(C::*)>& instance = nullptr, const pointer_base<owner>& instance_owner = nullptr) noexcept : pointer_base<V(C::*)>(instance), _class(instance_owner) {}
			complex_pointer(const complex_pointer& other) noexcept : complex_pointer(other._instance, other._class) {}
			template<typename U>
			complex_pointer(const complex_pointer<V(U::*), true>& other) noexcept
			requires(std::is_base_of_v<C, U> && is_const) : complex_pointer(static_cast<V(C::*)>(other.operator complex_pointer<V(U::*), true>::pointer()), other.get_owner()) {}
			template<typename U>
			complex_pointer(const complex_pointer<V(U::*), false>& other) noexcept
			requires(std::is_base_of_v<C, U>) : complex_pointer(static_cast<V(C::*)>(other.operator complex_pointer<V(U::*), false>::pointer()), other.get_owner()) {}

			template<typename U>
			complex_pointer<V(C::*), is_const>& operator=(const complex_pointer<V(U::*), false>& other) noexcept
			requires(std::is_base_of_v<C, U>) {
				_instance = other._instance;
				return *this;
			}
			template<typename U>
			complex_pointer<V(C::*), is_const>& operator=(const complex_pointer<V(U::*), true>& other) noexcept
			requires(std::is_base_of_v<C, U> && is_const) {
				_instance = other._instance;
				return *this;
			}
			template<typename U>
			complex_pointer<V(C::*), is_const>& operator=(complex_pointer<V(U::*), is_const>&& other) noexcept
			requires(std::is_base_of_v<C, U>) {
				std::swap(_instance, other._instance);
				return *this;
			}

			template<bool is_fail_safe = false, typename ...Args>
			requires(std::is_same_v<args_list<Args...>, field_function_a<pointer>>)
			std::conditional_t<is_field_function_v<pointer>, field_function_r<pointer>, std::add_lvalue_reference_t<std::conditional_t<is_const, std::add_const_t<V>, V>>> operator()(Args&&... args) const
			noexcept(is_fail_safe && (is_field_function_v<pointer> ? is_field_function_noexcept_v<pointer> : true)) {
				if constexpr (!is_fail_safe) {
					if (!(_class && _instance)) {
						throw nullptr_access(1);
					}
				}
				return pointer_base<V(C::*)>::operator()<true>(_class.operator*<true>(), std::forward<Args>(args)...);
			}

			void set_owner(const pointer_base<owner>& instance_owner = nullptr) noexcept {
				_class = instance_owner;
			}
			const pointer_base<owner>& get_owner() const noexcept {
				return _class;
			}
		};
}