#pragma once
#include <type_traits>
namespace dte_utils {
	template<typename T>
	inline constexpr bool has_not_equal_to_operator_v = requires (const T& t) { t != t; };

	template<typename T, bool has_not_equal_to = has_not_equal_to_operator_v<T>>
	struct is_nothrow_not_equal_to : std::false_type {};
	template<typename T>
	struct is_nothrow_not_equal_to<T, true> : std::bool_constant<noexcept(
			std::declval<const T&>() == std::declval<const T&>()
	)> {};
	template<typename T>
	inline constexpr bool is_nothrow_not_equal_to_v = is_nothrow_not_equal_to<T>::value;
}