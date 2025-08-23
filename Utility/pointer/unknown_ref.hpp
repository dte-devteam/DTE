#pragma once
#include "weak_ref.hpp"
#include "exceptions/pointer_exception.hpp"
#include "template_forwarding.hpp"
namespace dte_utils {
	template<typename T, typename RC = ref_counter>
	requires is_ref_counter_v<RC>
	struct unknown_ref : weak_ref<T, RC> {
		using size_type = weak_ref<T, RC>::size_type;
		using type = weak_ref<T, RC>::type;
		using pointer = weak_ref<T, RC>::pointer;
		protected:
			bool _strength;
			void _unknown_decrease() noexcept(std::is_nothrow_destructible_v<T>) {
				if (get_strength()) {
					this->_strong_decrease();
				}
			}
			void _unknown_increase() {
				if (get_strength()) {
					if (this->expired()) {
						throw bad_weak_ptr();
					}
					this->_counter->add_strong();
				}
			}
		public:
			unknown_ref(pointer instance = nullptr, bool strength = false) : weak_ref<T, RC>(instance), _strength(strength) {
				if (get_strength()) {
					this->_counter->add_strong();
				}
			}
			unknown_ref(const unknown_ref& other, bool strength = false) : weak_ref<T, RC>(other), _strength(strength) {
				_unknown_increase();
			}

			template<bool is_fail_safe = false>
			unknown_ref(const weak_ref<T, RC>& other, bool strength = false, template_forwarding<bool, is_fail_safe> = {}) noexcept(is_fail_safe) : weak_ref<T, RC>(other), _strength(strength) {
				if constexpr (is_fail_safe) {
					if (get_strength()) {
						this->_counter->add_strong();
					}
				}
				else {
					_unknown_increase();
				}
			}

			~unknown_ref() {
				_unknown_decrease();
			}

			bool get_strength() const noexcept {
				return _strength;
			}
			void set_strength(bool strength) {
				if (strength == _strength) {
					return;
				}
				_unknown_decrease();
				_strength = strength;
				_unknown_increase();
			}



			
			unknown_ref& operator=(pointer instance) {
				_unknown_decrease();
				this->_instance = instance;
				if (this->_counter->sub_weak()) {
					this->_counter = cnew<ref_counter>(1, get_strength());
				}
				return *this;
			}


			
			unknown_ref& operator=(const unknown_ref& other) {
				if (this == &other) {
					return *this;
				}
				_unknown_decrease();
				weak_ref<T, RC>::operator=(other);
				_unknown_increase();
				return *this;
			}

			unknown_ref& operator=(unknown_ref&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(this->_instance, other._instance);
				std::swap(this->_counter, other._counter);
				return *this;
			}

			unknown_ref& operator=(const weak_ref<T, RC>& other) {
				_unknown_decrease();
				weak_ref<T, RC>::operator=(other);
				_unknown_increase();
				return *this;
			}
	};
	template<typename T>
	using atomic_unknown_ref = unknown_ref<T, atomic_ref_counter>;
}