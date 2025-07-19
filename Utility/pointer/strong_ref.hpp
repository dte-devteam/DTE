#pragma once
#include "weak_ref.hpp"
#include "exceptions/pointer_exception.hpp"
namespace dte_utils {
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

			template<typename U>
			requires std::is_base_of_v<type, U> || std::is_same_v<type, U>
			strong_ref(const weak_ref<U, RC>& other) : weak_ref<T, RC>(other) {
				if (other.expired()) {
					throw bad_weak_ptr();
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
					this->_counter = cnew<RC>(
						static_cast<size_type>(1),
						static_cast<size_type>(1)
					);
				}
				return *this;
			}

			
			strong_ref& operator=(const strong_ref& other) {
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

			template<typename U>
			requires std::is_base_of_v<type, U> || std::is_same_v<type, U>
			strong_ref& operator=(const weak_ref<U, RC>& other) {
				if (reinterpret_cast<weak_ref<U, RC>*>(this) == &other) {
					return *this;
				}
				if (other.expired()) {
					throw bad_weak_ptr();
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