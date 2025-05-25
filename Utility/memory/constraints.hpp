#pragma once
#include <type_traits>
namespace dte_utils {

	template<typename U, typename T>
	concept copy_constructible = std::is_constructible_v<T, const U&>;
	template<typename U, typename T>
	concept move_constructible = std::is_constructible_v<T, U&&>;
	template<typename U, typename T>
	concept copy_or_move_constructible = copy_constructible<U, T> || move_constructible<U, T>;
	







	template<typename U, typename T>
	concept copy_assignable = requires(const U& u, T t) {
		t = u;
	};
}