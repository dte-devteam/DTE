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
			complex_pointer(const pointer_base<V(C::*)>& vc = nullptr, const pointer_base<owner>& o = nullptr) noexcept : pointer_base<V(C::*)>(vc), _class(o) {}
			complex_pointer(const complex_pointer& other) noexcept : complex_pointer(other._instance, other._class) {}
			template<typename U>
			complex_pointer(const complex_pointer<V(U::*), is_const>& other) noexcept
			requires(std::is_base_of_v<C, U>) : complex_pointer(static_cast<V(C::*)>(other.operator complex_pointer<V(U::*)>::pointer()), other.get_owner()) {}


			template<bool is_fail_safe = false, typename ...Args>
			requires(std::is_same_v<args_list<Args...>, field_function_a<pointer>>)
			std::conditional_t<is_field_function_v<pointer>, field_function_r<pointer>, std::add_lvalue_reference_t<std::add_const_t<V>>> operator()(Args&&... args) const
			noexcept(is_fail_safe && (is_field_function_v<pointer> ? is_field_function_noexcept_v<pointer> : true)) {
				if constexpr (!is_fail_safe) {
					if (!_class) {
						throw nullptr_access(1);
					}
				}
				return pointer_base<V(C::*)>::operator()<is_fail_safe>(_class.operator*<is_fail_safe>(), std::forward<Args>(args)...);
			}

			void set_owner(const pointer_base<owner>& o = nullptr) noexcept {
				_class = o;
			}
			const pointer_base<owner>& get_owner() const noexcept {
				return _class;
			}
		};
}