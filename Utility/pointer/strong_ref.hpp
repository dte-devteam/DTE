#pragma once
#include "weak_ref.hpp"
#include "exceptions/pointer_exception.hpp"
namespace dte_utils {
	template<allocatable T>
	struct strong_ref : weak_ref<T> {
		using size_type = weak_ref<T>::size_type;
		public:
			strong_ref(ref_pointer<T> instance = nullptr) : weak_ref<T>(instance) {
				++this->_counter->strong_owners;
			}
			strong_ref(const strong_ref& other) noexcept : weak_ref<T>(other) {
				++this->_counter->strong_owners;
			}

			template<allocatable U>
			requires std::is_base_of_v<T, U>
			strong_ref(const strong_ref<U>& other) : weak_ref<T>(other) {
				++this->_counter->strong_owners;
			}

			template<allocatable U>
			requires std::is_base_of_v<T, U> || std::is_same_v<T, U>
			strong_ref(const weak_ref<U>& other) : weak_ref<T>(other) {
				if (other.expired()) {
					throw bad_weak_ptr();
				}
				++this->_counter->strong_owners;
			}

			~strong_ref() {
				this->_strong_decrease();
			}


			template<allocatable U>
			requires std::is_base_of_v<T, U> || std::is_same_v<T, U>
			strong_ref& operator=(ref_pointer<U> instance) {
				this->_strong_decrease();
				this->_instance = instance;
				if (--this->_counter->weak_owners) {
					this->_counter = tmalloc<ref_counter>(1);
					place_at(this->_counter, static_cast<size_type>(1), static_cast<size_type>(1));
				}
				return *this;
			}

			template<allocatable U>
			requires std::is_base_of_v<T, U> || std::is_same_v<T, U>
			strong_ref& operator=(const strong_ref<U>& other) {
				if (reinterpret_cast<strong_ref<U>*>(this) == &other) {
					return *this;
				}
				this->_strong_decrease();
				weak_ref<T>::operator=(other);
				++this->_counter->strong_owners;
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

			template<allocatable U>
			requires std::is_base_of_v<T, U> || std::is_same_v<T, U>
			strong_ref& operator=(const weak_ref<U>& other) {
				if (reinterpret_cast<weak_ref<U>*>(this) == &other) {
					return *this;
				}
				if (other.expired()) {
					throw bad_weak_ptr();
				}
				this->_strong_decrease();
				weak_ref<T>::operator=(other);
				++this->_counter->strong_owners;
				return *this;
			}
	};
}