#pragma once
#include "type_properties.hpp"
namespace dte_utils {
	//Return false if T is not function pointer.
	template<typename>
	struct return_type : std::false_type {
		static constexpr bool is_noexcept = false;
	};
	template<typename R, typename ...Args>
	struct return_type<R(Args...)> : std::true_type {
		using type = typename R;
		static constexpr bool is_noexcept = false;
	};
	template<typename R, typename ...Args>
	struct return_type<R(Args...) noexcept> : std::true_type {
		using type = typename R;
		static constexpr bool is_noexcept = true;
	};
	template<typename T>
	using return_type_t = typename return_type<T>::type;
	template<typename T>
	inline constexpr bool return_type_v = return_type<T>::value;
	template<typename T>
	inline constexpr bool return_type_noexcept = return_type<T>::is_noexcept;



	template<typename T, typename ...Args>
	inline constexpr bool is_functor_v = requires(T t) {
		t.operator()(std::declval<Args>()...);
	};
	template<typename T, typename ...Args>
	using is_functor_t = decltype(std::declval<T>()(std::declval<Args>()...));
	template<typename T, bool>
	struct is_functor_noexcept {
		template<typename ...Args>
		static constexpr bool value = false;
	};
	template<typename T>
	struct is_functor_noexcept<T, true> {
		template<typename ...Args>
		static constexpr bool value = noexcept(
			std::declval<T>().operator()(std::declval<Args>()...)
		);
	};
	template<typename T, typename ...Args>
	inline constexpr bool is_functor_noexcept_v = is_functor_noexcept<T, is_functor_v<T, Args...>>::template value<Args...>;



	template<typename>
	struct is_field_function : std::false_type {
		using return_type	= int;	//we use non void type, because we can use references and function arguments
		using class_type	= int;	//we use non void type, because we can use references and function arguments
		using packed_args	= typename args_list<>;
		static constexpr bool is_noexcept = false;
		static constexpr bool is_const = false;
	};
	template<typename C, typename R, typename... Args>
	struct is_field_function<R(C::*)(Args...)> : std::true_type {
		using return_type	= typename R;
		using class_type	= typename C;
		using packed_args	= typename args_list<Args...>;
		static constexpr bool is_noexcept	= false;
		static constexpr bool is_const		= false;
	};
	template<typename C, typename R, typename... Args>
	struct is_field_function<R(C::*)(Args...) noexcept> : std::true_type {
		using return_type = typename R;
		using class_type = typename C;
		using packed_args = typename args_list<Args...>;
		static constexpr bool is_noexcept	= true;
		static constexpr bool is_const		= false;
	};
	template<typename C, typename R, typename... Args>
	struct is_field_function<R(C::*)(Args...) const> : std::true_type {
		using return_type = typename R;
		using class_type = typename C;
		using packed_args = typename args_list<Args...>;
		static constexpr bool is_noexcept	= false;
		static constexpr bool is_const		= true;
	};
	template<typename C, typename R, typename... Args>
	struct is_field_function<R(C::*)(Args...) const noexcept> : std::true_type {
		using return_type = typename R;
		using class_type = typename C;
		using packed_args = typename args_list<Args...>;
		static constexpr bool is_noexcept	= true;
		static constexpr bool is_const		= true;
	};
	template<typename T>
	inline constexpr bool is_field_function_v = is_field_function<T>::value;
	template<typename T>
	inline constexpr bool is_field_function_noexcept_v = is_field_function<T>::is_noexcept;
	template<typename T>
	inline constexpr bool is_field_function_const_v = is_field_function<T>::is_const;
	template<typename T>
	using field_function_r = typename is_field_function<T>::return_type;
	template<typename T>
	using field_function_c = typename is_field_function<T>::class_type;
	template<typename T>
	using field_function_a = typename is_field_function<T>::packed_args;
}