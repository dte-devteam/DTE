#pragma once
#include "exceptions/memory_exception.hpp"
#include "type_properties.hpp"
#include <concepts>
namespace dte_utils {
	template<typename T>
	constexpr inline void* downgrade_ptr(T* ptr) noexcept {
		return static_cast<void*>(const_cast<std::remove_cv_t<T>*>(ptr));
	}
	template<typename T>
	constexpr inline std::remove_const_t<T>* remove_const_ptr(T* ptr) noexcept {
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
			constexpr raw_pointer(pointer instance = nullptr) noexcept : _instance(instance) {}
			constexpr raw_pointer(const raw_pointer& other) noexcept : _instance(other._instance) {}
			template<typename U>
			constexpr raw_pointer(const raw_pointer<U>& other) noexcept
			requires(std::is_constructible_v<pointer, typename raw_pointer<U>::pointer>) : _instance(other.operator raw_pointer<U>::pointer()) {}

			constexpr explicit operator pointer() const noexcept {
				return _instance;
			}
			constexpr explicit operator bool() const noexcept {
				return _instance;
			}
			constexpr pointer operator->() const noexcept {
				return _instance;
			}
			template<bool is_fail_safe = false>
			constexpr std::add_lvalue_reference_t<type> operator*() const noexcept(is_fail_safe)
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return *_instance;
			}
			template<bool is_fail_safe = false>
			constexpr std::add_lvalue_reference_t<type> operator[](ptrdiff_t index) const noexcept(is_fail_safe)
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return (*this + index).operator*<is_fail_safe>();
			}

			template<typename U>
			constexpr raw_pointer& operator=(U ptr) noexcept
			requires(std::is_assignable_v<pointer&, U>) {
				_instance = ptr;
				return *this;
			}
			template<typename U>
			constexpr raw_pointer& operator=(const raw_pointer<U>& other) noexcept
			requires(std::is_assignable_v<pointer&, typename raw_pointer<U>::pointer>) {
				*this = other.operator raw_pointer<U>::pointer();
				return *this;
			}
			constexpr raw_pointer& operator=(const raw_pointer& other) noexcept {
				_instance = other._instance;
				return *this;
			}
			constexpr raw_pointer& operator=(raw_pointer&& other) noexcept {
				std::swap(_instance, other._instance);
				return *this;
			}

			constexpr raw_pointer operator+(ptrdiff_t add) const noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance + add };
			}
			constexpr raw_pointer& operator+=(ptrdiff_t add) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				_instance += add;
				return *this;
			}

			constexpr raw_pointer operator-(ptrdiff_t sub) const noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance - sub };
			}
			constexpr raw_pointer& operator-=(ptrdiff_t sub) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				_instance -= sub;
				return *this;
			}
			constexpr ptrdiff_t operator-(pointer ptr) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return _instance - ptr;
			}
			constexpr ptrdiff_t operator-(const raw_pointer& other) const noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return _instance - other._instance;
			}

			constexpr raw_pointer& operator++() noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				++_instance;
				return *this;
			}
			constexpr raw_pointer operator++(int) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance++ };
			}

			constexpr raw_pointer& operator--() noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				--_instance;
				return *this;
			}
			constexpr raw_pointer operator--(int) noexcept
			requires(!(std::is_void_v<type> || is_field_v<type> || return_type_v<type>)) {
				return { _instance-- };
			}

			template<typename U>
			constexpr bool operator==(U* ptr) const noexcept
			requires(!is_field_v<type>) {
				//we force comparement by casting everything to void*
				return downgrade_ptr(_instance) == downgrade_ptr(ptr);
			}
			template<typename V, typename C>
			constexpr bool operator==(V(C::*field_ptr)) const noexcept
			requires(std::equality_comparable_with<pointer, V(C::*)>) {
				return _instance == field_ptr;
			}
			template<typename U>
			constexpr bool operator==(const raw_pointer<U>& other) const noexcept 
			requires(!is_field_v<type>) {
				return *this == other.operator raw_pointer<U>::pointer();
			}
			template<typename V, typename C>
			constexpr bool operator==(const raw_pointer<V(C::*)>& other) const noexcept
			requires(std::equality_comparable_with<pointer, typename raw_pointer<V(C::*)>::pointer>) {
				return *this == other.operator raw_pointer<V(C::*)>::pointer();
			}

			template<typename U>
			constexpr bool operator>(U* ptr) const noexcept
			requires(!is_field_v<type>) {
				return downgrade_ptr(_instance) > downgrade_ptr(ptr);
			}
			template<typename V, typename C>
			constexpr bool operator>(V(C::* field_ptr)) const noexcept
			requires(std::equality_comparable_with<pointer, V(C::*)>) {
				return _instance == field_ptr;
			}
			template<typename U>
			constexpr bool operator>(const raw_pointer<U>& other) const noexcept
			requires(!is_field_v<type>) {
				return *this > other.operator raw_pointer<U>::pointer();
			}
			template<typename V, typename C>
			constexpr bool operator>(const raw_pointer<V(C::*)>& other) const noexcept
			requires(std::equality_comparable_with<pointer, typename raw_pointer<V(C::*)>::pointer>) {
				return *this > other.operator raw_pointer<V(C::*)>::pointer();
			}

			template<typename U>
			constexpr bool operator<(U* ptr) const noexcept
			requires(!is_field_v<type>) {
				return downgrade_ptr(_instance) < downgrade_ptr(ptr);
			}
			template<typename V, typename C>
			constexpr bool operator<(V(C::* field_ptr)) const noexcept
			requires(std::equality_comparable_with<pointer, V(C::*)>) {
				return _instance < field_ptr;
			}
			template<typename U>
			constexpr bool operator<(const raw_pointer<U>& other) const noexcept
			requires(!is_field_v<type>) {
				return *this < other.operator raw_pointer<U>::pointer();
			}
			template<typename V, typename C>
			constexpr bool operator<(const raw_pointer<V(C::*)>& other) const noexcept
			requires(std::equality_comparable_with<pointer, typename raw_pointer<V(C::*)>::pointer>) {
				return *this < other.operator raw_pointer<V(C::*)>::pointer();
			}

			template<typename U>
			constexpr bool operator<=(U* ptr) const noexcept
			requires(!is_field_v<type>) {
				return !(*this > ptr);
			}
			template<typename V, typename C>
			constexpr bool operator<=(V(C::* field_ptr)) const noexcept
			requires(std::equality_comparable_with<pointer, V(C::*)>) {
				return !(*this > field_ptr);
			}
			template<typename U>
			constexpr bool operator<=(const raw_pointer<U>& other) const noexcept
			requires(!is_field_v<type>) {
				return !(*this > other);
			}
			template<typename V, typename C>
			constexpr bool operator<=(const raw_pointer<V(C::*)>& other) const noexcept
			requires(std::equality_comparable_with<pointer, typename raw_pointer<V(C::*)>::pointer>) {
				return !(*this > other);
			}

			template<typename U>
			constexpr bool operator>=(U* ptr) const noexcept
			requires(!is_field_v<type>) {
				return !(*this < ptr);
			}
			template<typename V, typename C>
			constexpr bool operator>=(V(C::* field_ptr)) const noexcept
			requires(std::equality_comparable_with<pointer, V(C::*)>) {
				return !(*this < field_ptr);
			}
			template<typename U>
			constexpr bool operator>=(const raw_pointer<U>& other) const noexcept
			requires(!is_field_v<type>) {
				return !(*this < other);
			}
			template<typename V, typename C>
			constexpr bool operator>=(const raw_pointer<V(C::*)>& other) const noexcept
			requires(std::equality_comparable_with<pointer, typename raw_pointer<V(C::*)>::pointer>) {
				return !(*this < other);
			}
			
			template<bool is_fail_safe = false>
			constexpr std::conditional_t<is_field_v<type> || return_type_v<type>, pointer, std::add_lvalue_reference_t<type>> get_value() const noexcept(is_fail_safe)
			requires(!std::is_void_v<type>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				if constexpr (is_field_v<type> || return_type_v<type>) {
					return _instance;
				}
				else {
					return *_instance;
				}
			}
	};
	template<typename T>
	constexpr inline raw_pointer<T> remove_const_ptr_base(const raw_pointer<T>& ptr) noexcept {
		return ptr;
	}
	template<typename T>
	constexpr inline raw_pointer<T> remove_const_ptr_base(const raw_pointer<const T>& ptr) noexcept {
		return raw_pointer<T>(remove_const_ptr(ptr.operator raw_pointer<const T>::pointer()));
	}
	template<typename T, typename F>
	constexpr inline raw_pointer<T> static_cast_ptr_base(const raw_pointer<F>& ptr) noexcept
	requires(is_static_castable_v<typename raw_pointer<F>::pointer, typename raw_pointer<T>::pointer>) {
		return raw_pointer<T>(static_cast<raw_pointer<T>::pointer>(ptr.operator raw_pointer<F>::pointer()));
	}
}