#pragma once
#include <type_traits>
namespace dte_utils {
	template<typename T, typename F>
	inline constexpr bool is_static_castable_v = requires (F& from) {
		static_cast<T>(from);
	};

	template<typename T, typename U = T>
	inline constexpr bool has_equal_to_operator_v = requires (const T& t, const U& u) { t == u; };
	template<typename T, typename U, bool has_not_equal_to = has_equal_to_operator_v<T, U>>
	struct is_nothrow_equal_to : std::false_type {};
	template<typename T, typename U>
	struct is_nothrow_equal_to<T, U, true> : std::bool_constant<noexcept(
		std::declval<const T&>() == std::declval<const U&>()
	)> {};
	template<typename T, typename U = T>
	inline constexpr bool is_nothrow_equal_to_v = is_nothrow_equal_to<T, U>::value;


	template<typename T, typename U = T>
	inline constexpr bool has_not_equal_to_operator_v = requires (const T& t, const U& u) { t != u; };
	template<typename T, typename U, bool has_not_equal_to = has_not_equal_to_operator_v<T, U>>
	struct is_nothrow_not_equal_to : std::false_type {};
	template<typename T, typename U>
	struct is_nothrow_not_equal_to<T, U, true> : std::bool_constant<noexcept(
			std::declval<const T&>() != std::declval<const U&>()
	)> {};
	template<typename T, typename U = T>
	inline constexpr bool is_nothrow_not_equal_to_v = is_nothrow_not_equal_to<T, U>::value;

	template<typename>
	struct is_field : std::false_type {
		using field_type = int;	//we use non void type, because we can use references and function arguments
		using class_type = int;	//we use non void type, because we can use references and function arguments
	};
	template<typename C, typename T>
	struct is_field<T(C::*)> : std::true_type {
		using field_type = T;
		using class_type = C;
	};
	template<typename T>
	inline constexpr bool is_field_v = is_field<T>::value;
	template<typename T>
	using field_t = typename is_field<T>::field_type;
	template<typename T>
	using field_c = typename is_field<T>::class_type;

	template<typename T>
	struct drop_pointer {
		using type = T;
		static constexpr size_t value = 0;
	};
	template<typename T>
	using drop_pointer_t = typename drop_pointer<T>::type;
	template<typename T>
	inline constexpr size_t drop_pointer_v = drop_pointer<T>::value;
	template<typename T>
	struct drop_pointer<T*> {
		using type = drop_pointer_t<T>;
		static constexpr size_t value = 1 + drop_pointer_v<T>;
	};
	template<typename T>
	struct drop_pointer<T* const> {
		using type = drop_pointer_t<T>;
		static constexpr size_t value = 1 + drop_pointer_v<T>;
	};
	template<typename T>
	struct drop_pointer<T* volatile> {
		using type = drop_pointer_t<T>;
		static constexpr size_t value = 1 + drop_pointer_v<T>;
	};
	template<typename T>
	struct drop_pointer<T* const volatile> {
		using type = drop_pointer_t<T>;
		static constexpr size_t value = 1 + drop_pointer_v<T>;
	};

	template<typename T, size_t N>
	struct set_pointer {
		using type = typename set_pointer<T*, N - 1>::type;
	};
	template<typename T>
	struct set_pointer<T, 0> {
		using type = T;
	};
	template<typename T, size_t N>
	using set_pointer_t = set_pointer<T, N>::type;

	template<typename ...Args>
	struct args_list {};
}