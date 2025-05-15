#pragma once
#include <concepts>
#include <type_traits>
namespace dte_utils {
	template<typename T>
	concept sizeof_limits = !(std::is_void_v<T> || std::is_unbounded_array_v<T>);

	template<typename U, typename T>
	concept copy_constructible = std::is_constructible_v<T, const U&>;
	template<typename U, typename T>
	concept move_constructible = std::is_constructible_v<T, U&&>;
	template<typename U, typename T>
	concept copy_or_move_constructible = copy_constructible<U, T> || move_constructible<U, T>;
	



	template<typename T>
	concept allocatable = copy_or_move_constructible<T, T> && std::is_destructible_v<T>;

	template<typename T>
	concept nullable = requires(T t) {
		T(0);
		t = 0;
		t == 0;
		t != 0;
	};

	template<typename U, typename T>
	concept copy_assignable = requires(const U & u, T t) {
		t = u;
	};
}