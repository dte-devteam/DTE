#pragma once
#include "type_properties.hpp"
#include "function_traits.hpp"
#include "exceptions/memory_exception.hpp"
namespace dte_utils {
	template<typename T>
	inline void* downgrade_ptr(T* ptr) noexcept {
		return static_cast<void*>(const_cast<std::remove_cv_t<T>*>(ptr));
	}
	template<typename T>
	struct raw_pointer {
		using pointer	= typename std::conditional_t<is_field_v<drop_pointer_t<T>>, T, T*>;
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
	template<typename T>
	struct pointer_base : raw_pointer<T> {
		using type		= typename T;
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
			//member pointers
			/*
			template<bool is_fail_safe = false, typename ...Args>
			requires(std::is_same_v<field_function_a<type>, args_list<Args...>>) 
			field_function_r<type> operator()(field_function_c<type>& c, Args&&... args) const
			noexcept(is_fail_safe && is_field_function_noexcept_v<type>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return (c.*_instance)(std::forward<Args>(args)...);
			}
			template<bool is_fail_safe = false, typename ...Args>
			requires(std::is_same_v<field_function_a<type>, args_list<Args...>> && is_field_function_const_v<type>)
			field_function_r<type> operator()(const field_function_c<type>& c, Args&&... args) const
			noexcept(is_fail_safe && is_field_function_noexcept_v<type>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return (c.*_instance)(std::forward<Args>(args)...);
			}


			template<bool is_fail_safe = false>
			std::conditional_t<return_type_v<field_t<type>>, int, field_t<type>>& operator()(field_c<type>& c) const
			noexcept(is_fail_safe)
			requires(!is_field_function_v<type> && is_field_v<type>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return c.*_instance;
			}
			template<bool is_fail_safe = false>
			const std::conditional_t<return_type_v<field_t<type>>, int, field_t<type>>& operator()(const field_c<type>& c) const
			noexcept(is_fail_safe)
			requires(!is_field_function_v<type>&& is_field_v<type>) {
				if constexpr (!is_fail_safe) {
					if (!_instance) {
						throw nullptr_access();
					}
				}
				return c.*_instance;
			}
			*/
	};

	//specialisation fails - it doesn`t differ Val(Class::*) and Ret(Class::*)(Args...)

	template<typename Class, typename Val>
	struct pointer_base<Val(Class::*)> : raw_pointer<Val(Class::*)> {
		using pointer = raw_pointer<Val(Class::*)>::pointer;
		using raw_pointer<Val(Class::*)>::raw_pointer;
		public:
			template<bool is_fail_safe = false, typename ...Args>
			requires(std::is_same_v<args_list<Args...>, field_function_a<pointer>>)
			std::conditional_t<return_type_v<Val>, field_function_r<pointer>, std::add_lvalue_reference_t<Val>> operator()(Class& c, Args&&... args) const {
				if constexpr (return_type_v<Val>) {
					return (c.*_instance)(std::forward<Args>(args)...);
				}
				else {
					return c.*_instance;
				}
			}
	};
	/*
	template<typename Class, typename Ret, typename... Args>
	requires(is_field_function_v<typename raw_pointer<Ret(Class::*)(Args...)>::pointer>)
	struct pointer_base<Ret(Class::*)(Args...)> : raw_pointer<Ret(Class::*)(Args...)> {
		using raw_pointer<Ret(Class::*)(Args...)>::raw_pointer;
		public:
			Ret operator()(Class& c, Args&&... args) const {
				return (c.*_instance)(std::forward<Args>(args)...);
			}
	};
	*/
	template<typename T>
	inline T* remove_const_ptr(T* ptr) noexcept {
		return ptr;
	}
	template<typename T>
	inline T* remove_const_ptr(const T* ptr) noexcept {
		return const_cast<T*>(ptr);
	}
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