#pragma once
#include "type_properties.hpp"
namespace dte_utils {
	template<typename T>
	inline void* downgrade_ptr(T* ptr) noexcept {
		return static_cast<void*>(const_cast<std::remove_cv_t<T>*>(ptr));
	}
	template<typename T>
	inline T* remove_const_ptr(T* ptr) noexcept {
		return ptr;
	}
	template<typename T>
	inline T* remove_const_ptr(const T* ptr) noexcept {
		return const_cast<T*>(ptr);
	}
	template<typename T>
	struct raw_pointer {
		using pointer	= typename std::conditional_t<is_field_v<drop_pointer_t<T>>, T, T*>;
		using type		= typename std::remove_pointer_t<pointer>;
		protected:
			pointer _instance;
		public:
			raw_pointer(pointer instance = nullptr) noexcept : _instance(instance) {}
			raw_pointer(const raw_pointer& other) noexcept : _instance(other._instance) {}
			template<typename U>
			raw_pointer(const raw_pointer<U>& other) noexcept
				requires(std::is_convertible_v<typename raw_pointer<U>::pointer, pointer>) : _instance(other.operator raw_pointer<U>::pointer()) {}

			explicit operator pointer() const noexcept {
				return _instance;
			}
			explicit operator bool() const noexcept {
				return _instance;
			}

			template<typename U>
			raw_pointer& operator=(U* ptr) noexcept
				requires(std::is_convertible_v<U*, pointer>) {
				_instance = ptr;
				return *this;
			}
			template<typename U>
			raw_pointer& operator=(const raw_pointer<U>& other) noexcept
				requires(std::is_convertible_v<typename raw_pointer<U>::pointer, pointer>) {
				_instance = other.operator raw_pointer<U>::pointer();
				return *this;
			}

			template<typename U>
			bool operator==(U* ptr) const noexcept {
				//we force comparement by casting everything to void*
				return downgrade_ptr(_instance) == downgrade_ptr(ptr);
			}
			template<typename U>
			bool operator!=(U* ptr) const noexcept {
				return !(*this == ptr);
			}

			template<typename U>
			bool operator==(const raw_pointer<U>& other) const noexcept {
				return *this == other.operator raw_pointer<U>::pointer();
			}
			template<typename U>
			bool operator!=(const raw_pointer<U>& other) const noexcept {
				return !(*this == other);
			}
	};
}