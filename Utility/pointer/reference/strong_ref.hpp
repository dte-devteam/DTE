#pragma once
#include "weak_ref.hpp"
#include "exceptions/pointer_exception.hpp"
#include "template_forwarding.hpp"
namespace dte_utils {
	template<typename T, typename RC, bool = false>
	requires is_ref_counter_v<RC>
	struct strong_ref_no_event : weak_ref_no_event<T, RC> {
		using raw_type	= typename weak_ref_no_event<T, RC>::raw_type;
		using type		= typename weak_ref_no_event<T, RC>::type;
		using pointer	= typename weak_ref_no_event<T, RC>::pointer;
		using owner		= typename weak_ref_no_event<T, RC>::owner;
		using size_type	= typename weak_ref_no_event<T, RC>::size_type;
		public:
			strong_ref_no_event(const complex_pointer<type>& instance = {}) : weak_ref_no_event<type, RC>(instance, 1, 1) {}
			template<typename U, bool is_fail_safe = false>
			strong_ref_no_event(const weak_ref_no_event<U, RC>& other, template_forwarding<bool, is_fail_safe> = {})
			requires(is_static_castable_v<pointer, typename weak_ref_no_event<U, RC>::pointer>) : weak_ref_no_event<type, RC>(other) {
				if constexpr (!is_fail_safe) {
					if (other.expired()) {
						throw bad_weak_ptr();
					}
				}
				_counter->add_strong();
			}
			template<typename U, bool is_fail_safe = false>
			strong_ref_no_event(const weak_ref_no_event<U, RC>& other, template_forwarding<bool, is_fail_safe> = {})
			requires(!is_static_castable_v<pointer, typename weak_ref_no_event<U, RC>::pointer>) = delete;
			template<typename U>
			strong_ref_no_event(const strong_ref_no_event<U, RC>& other) noexcept
			requires(is_static_castable_v<pointer, typename weak_ref_no_event<U, RC>::pointer>) : weak_ref_no_event<type, RC>(other) {
				_counter->add_strong();
			}
			template<typename U, typename ORC>
			strong_ref_no_event(const strong_ref_no_event<U, ORC>& other) = delete;
			~strong_ref_no_event() {
				_strong_decrease();
			}
			/*
			template<typename U, bool is_fail_safe = false>
			strong_ref_no_event& operator=(const weak_ref_no_event<U, RC>& other)
			requires(is_static_castable<pointer, typename weak_ref_no_event<U, RC>::pointer>) {
				if constexpr (!is_fail_safe) {
					if (other.expired()) {
						throw bad_weak_ptr();
					}
				}
				if (*this == other) {
					return *this;
				}
				_strong_decrease();
				weak_ref_no_event<type, RC>::set_value<false>(other);
				_instance = other.operator weak_ref_no_event<U, RC>::pointer();
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(1, 1);
				}
				else {
					_counter->add_strong();
				}
				return *this;
			}*/
	};






	template<typename T, typename RC = ref_counter>
	requires is_ref_counter_v<RC>
	struct strong_ref : weak_ref<T, RC> {
		using size_type	= typename weak_ref<T, RC>::size_type;
		using type		= typename weak_ref<T, RC>::type;
		using pointer	= typename weak_ref<T, RC>::pointer;
		public:
			strong_ref(pointer instance = nullptr) : weak_ref<T, RC>(instance) {
				_counter->add_strong();
			}
			strong_ref(const strong_ref& other) noexcept : weak_ref<T, RC>(other) {
				_counter->add_strong();
			}

			template<typename U, bool is_fail_safe = false>
			strong_ref(const weak_ref<U, RC>& other, template_forwarding<bool, is_fail_safe> = {}) noexcept(is_fail_safe) 
			requires(std::is_convertible_v<typename weak_ref<U, RC>::pointer, pointer>) : weak_ref<T, RC>(other) {
				if constexpr (!is_fail_safe) {
					if (other.expired()) {
						throw bad_weak_ptr();
					}
				}
				_counter->add_strong();
			}
			template<typename U>
			strong_ref(const raw_pointer<U>& other)
			requires(std::is_convertible_v<typename raw_pointer<U>::pointer, pointer>) : strong_ref(other.operator raw_pointer<U>::pointer()) {}

			~strong_ref() {
				_strong_decrease();
			}


			strong_ref& operator=(pointer instance) {
				_strong_decrease();
				_instance = instance;
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(1, 1);
				}
				else {
					_counter->add_strong();
				}
				return *this;
			}

			
			strong_ref& operator=(const strong_ref& other)
			noexcept(
				std::is_nothrow_destructible_v<T> && 
				std::is_nothrow_destructible_v<RC>
			) {
				if (this == &other) {
					return *this;
				}
				_strong_decrease();
				weak_ref<T, RC>::operator=(other);
				_counter->add_strong();
				return *this;
			}

			strong_ref& operator=(strong_ref&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_instance, other._instance);
				std::swap(_counter, other._counter);
				return *this;
			}

			template<bool is_fail_safe = false>
			strong_ref& operator=(const weak_ref<T, RC>& other)
			noexcept(
				std::is_nothrow_destructible_v<T> &&
				std::is_nothrow_destructible_v<RC> && 
				is_fail_safe
			) {
				if constexpr (!is_fail_safe) {
					if (other.expired()) {
						throw bad_weak_ptr();
					}
				}
				_strong_decrease();
				weak_ref<T, RC>::operator=(other);
				_counter->add_strong();
				return *this;
			}

			template<typename U>
			strong_ref& operator=(const pointer_base<U>& other)
			requires(std::is_convertible_v<typename pointer_base<U>::pointer, pointer>) {
				return *this = other.operator->();
			}
	};
	template<typename T>
	using atomic_strong_ref = strong_ref<T, atomic_ref_counter>;
}