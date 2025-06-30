#pragma once
#include <type_traits>
namespace dte_utils {
	template<typename T, typename = void>
	struct has_not_equal_to_operator : std::false_type {};
	template<typename T>
	struct has_not_equal_to_operator<T, std::void_t<decltype(
		std::declval<const T&>() != std::declval<const T&>()
	)>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_not_equal_to_operator_v = has_not_equal_to_operator<T>::value;

	template<typename T, bool has_not_equal_to = has_not_equal_to_operator_v<T>>
	struct is_nothrow_not_equal_to : std::false_type {};
	template<typename T>
	struct is_nothrow_not_equal_to<T, true> : std::bool_constant<noexcept(
			std::declval<const T&>() == std::declval<const T&>()
	)> {};
	template<typename T>
	inline constexpr bool is_nothrow_not_equal_to_v = is_nothrow_not_equal_to<T>::value;
}