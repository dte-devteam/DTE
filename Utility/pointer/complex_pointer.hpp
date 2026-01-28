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
			complex_pointer(const pointer_base<type>& instance = nullptr, const pointer_base<owner>& instance_owner = nullptr) noexcept : pointer_base<type>(instance), _class(instance_owner) {}
			complex_pointer(const complex_pointer& other) noexcept : complex_pointer(other, other.get_owner()) {}
			

			template<typename U>
			complex_pointer(const complex_pointer<U, is_const>& other) noexcept
			requires(!std::is_same_v<pointer, typename complex_pointer<U, is_const>::pointer> && std::is_constructible_v<pointer, typename complex_pointer<U, is_const>::pointer>) : 
				pointer_base<type>(other), 
				_class(other.get_owner()) {}

			template<typename U, bool other_const>
			complex_pointer(const complex_pointer<U, other_const>& other) noexcept
			requires(is_const > other_const && std::is_constructible_v<pointer, typename complex_pointer<U, other_const>::pointer>) : complex_pointer(other, other.get_owner()) {}
			
			
			//template<typename U>
			//complex_pointer(const complex_pointer<U, is_const>& instance)
			//requires(!std::is_same_v<U, type>) : pointer_base<type>(instance), _class(instance.get_owner()) {}
			template<typename U, bool other_const>
			complex_pointer(const complex_pointer<U, other_const>& other) noexcept
			requires(is_const > other_const) : complex_pointer(static_cast<pointer>(other.operator complex_pointer<U, other_const>::pointer()), other.get_owner()) {}
			//template<typename U, bool other_const>
			//complex_pointer(const complex_pointer<U, other_const>& other) noexcept
			//requires(is_const >= other_const && !is_static_castable_v<pointer, typename complex_pointer<U, other_const>::pointer>) = delete;
			
			//template<typename U, bool other_const>
			//complex_pointer(const complex_pointer<U, other_const>& other)
			//requires(is_const < other_const) = delete;
			
			/*
			template<bool other_const>
			complex_pointer<type, is_const>& operator=(const complex_pointer<type, other_const>& other) noexcept
			requires(is_const >= other_const) {
				_instance = other.operator complex_pointer<type, other_const>::pointer();
				_class = other.get_owner();
				return *this;
			}*/
			complex_pointer<type, is_const>& operator=(complex_pointer&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_instance, other._instance);
				std::swap(_class, other._class);
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
				return pointer_base<type>::operator()<true>(_class.operator*<true>(), std::forward<Args>(args)...);
			}

			void set_owner(const pointer_base<owner>& instance_owner = nullptr) noexcept {
				_class = instance_owner;
			}
			const pointer_base<owner>& get_owner() const noexcept {
				return _class;
			}
			void set_field(pointer instance) noexcept {
				_instance = instance;
			}
	};
}