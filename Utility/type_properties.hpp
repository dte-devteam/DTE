#pragma once
#include <type_traits>
namespace dte_utils {
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
}