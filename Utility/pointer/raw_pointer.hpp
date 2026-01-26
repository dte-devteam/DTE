#pragma once
#include <string>

#include "exceptions/memory_exception.hpp"
#include "type_properties.hpp"
namespace dte_utils {
	template<typename T>
	inline void* downgrade_ptr(T* ptr) noexcept {
		return static_cast<void*>(const_cast<std::remove_cv_t<T>*>(ptr));
	}
	template<typename T>
	inline std::remove_const_t<T>* remove_const_ptr(T* ptr) noexcept {
		return const_cast<std::remove_const_t<T>*>(ptr);
	}
	template<typename T>
	struct raw_pointer {
		using raw_type	= typename drop_pointer_t<T>;
		using pointer	= typename std::conditional_t<is_field_v<raw_type>, T, T*>;
		using type		= typename std::remove_pointer_t<pointer>;
		protected:
			pointer _instance;
		public:
			raw_pointer(pointer instance = nullptr) noexcept : _instance(instance) {}
			raw_pointer(const raw_pointer& other) noexcept : _instance(other._instance) {}
			template<typename U>
			raw_pointer(const raw_pointer<U>& other) noexcept
			requires(is_static_castable_v<pointer, typename raw_pointer<U>::pointer>) : _instance(static_cast<pointer>(other.operator raw_pointer<U>::pointer())) {}

			explicit operator pointer() const noexcept {
				return _instance;
			}
			explicit operator bool() const noexcept {
				return _instance;
			}

			template<typename U>
			raw_pointer& operator=(U ptr) noexcept
			requires(is_static_castable_v<pointer, U>) {
				_instance = static_cast<pointer>(ptr);
				return *this;
			}
			template<typename U>
			raw_pointer& operator=(const raw_pointer<U>& other) noexcept
			requires(is_static_castable_v<pointer, typename raw_pointer<U>::pointer>) {
				*this = other.operator raw_pointer<U>::pointer();
				return *this;
			}

			template<typename U>
			bool operator==(U* ptr) const noexcept 
			requires(!is_field_v<type>) {
				//we force comparement by casting everything to void*
				return downgrade_ptr(_instance) == downgrade_ptr(ptr);
			}
			template<typename V, typename C>
			bool operator==(V(C::* field_ptr)) const noexcept
			requires(is_static_castable_v<pointer, V(C::*)>) {
				return _instance == static_cast<pointer>(field_ptr);
			}
			template<typename U>
			bool operator==(const raw_pointer<U>& other) const noexcept 
			requires(is_static_castable_v<pointer, typename raw_pointer<U>::pointer>) {
				return *this == other.operator raw_pointer<U>::pointer();
			}

			template<bool is_fail_safe = false>
			std::conditional_t<std::is_same_v<raw_type, type>, std::add_lvalue_reference_t<type>, raw_pointer<std::conditional_t<is_field_v<raw_type>, type, std::remove_pointer_t<type>>>> get_value() noexcept(is_fail_safe) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				if constexpr (is_field_v<type>) {
					return _instance;
				}
				else {
					return *_instance;
				}
			}
			template<bool is_fail_safe = false>
			std::conditional_t<std::is_same_v<raw_type, type>, std::conditional_t<is_field_v<type>, std::add_lvalue_reference_t<std::add_const_t<type>>, std::add_lvalue_reference_t<type>>, raw_pointer<std::conditional_t<is_field_v<raw_type>, type, std::remove_pointer_t<type>>>> get_value() const noexcept(is_fail_safe) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				if constexpr (is_field_v<type>) {
					return _instance;
				}
				else {
					return *_instance;
				}
			}
	};
	template<typename T>
	inline raw_pointer<T> remove_const_ptr_base(const raw_pointer<T>& ptr) noexcept {
		return ptr;
	}
	template<typename T>
	inline raw_pointer<T> remove_const_ptr_base(const raw_pointer<const T>& ptr) noexcept {
		return raw_pointer<T>(remove_const_ptr(ptr.operator raw_pointer<const T>::pointer()));
	}
	template<typename T, typename F>
	inline raw_pointer<T> static_cast_ptr_base(const raw_pointer<F>& ptr) noexcept
	requires(is_static_castable_v<typename raw_pointer<F>::pointer, typename raw_pointer<T>::pointer>) {
		return raw_pointer<T>(static_cast<raw_pointer<T>::pointer>(ptr.operator raw_pointer<F>::pointer()));
	}
}