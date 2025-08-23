#pragma once
#include "weak_ref.hpp"
#include "exceptions/pointer_exception.hpp"
#include "template_forwarding.hpp"
namespace dte_utils {
	template<typename T, T v>
	struct bool_h {};
	template<typename T, typename RC = ref_counter>
	requires is_ref_counter_v<RC>
	struct strong_ref : weak_ref<T, RC> {
		using size_type = weak_ref<T, RC>::size_type;
		using type = weak_ref<T, RC>::type;
		using pointer = weak_ref<T, RC>::pointer;
		public:
			strong_ref(pointer instance = nullptr) : weak_ref<T, RC>(instance) {
				this->_counter->add_strong();
			}
			strong_ref(const strong_ref& other) noexcept : weak_ref<T, RC>(other) {
				this->_counter->add_strong();
			}

			template<bool is_fail_safe = false>
			strong_ref(const weak_ref<T, RC>& other, template_forwarding<bool, is_fail_safe> = {}) noexcept(is_fail_safe) : weak_ref<T, RC>(other) {
				if constexpr (!is_fail_safe) {
					if (other.expired()) {
						throw bad_weak_ptr();
					}
				}
				this->_counter->add_strong();
			}

			~strong_ref() {
				this->_strong_decrease();
			}


			strong_ref& operator=(pointer instance) {
				this->_strong_decrease();
				this->_instance = instance;
				if (this->_counter->sub_weak()) {
					this->_counter = cnew<RC>(1, 1);
				}
				else {
					this->_counter->add_strong();
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
				this->_strong_decrease();
				weak_ref<T, RC>::operator=(other);
				this->_counter->add_strong();
				return *this;
			}

			strong_ref& operator=(strong_ref&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(this->_instance, other._instance);
				std::swap(this->_counter, other._counter);
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
				this->_strong_decrease();
				weak_ref<T, RC>::operator=(other);
				this->_counter->add_strong();
				return *this;
			}
	};
	template<typename T>
	using atomic_strong_ref = strong_ref<T, atomic_ref_counter>;
}