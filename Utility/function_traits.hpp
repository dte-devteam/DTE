#pragma once
#include <type_traits>
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
}