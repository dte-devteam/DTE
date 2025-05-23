#pragma once
#include "weak_ref.hpp"
#include "exceptions/pointer_exception.hpp"
namespace dte_utils {
	template<typename T>
	requires std::is_destructible_v<T>
	struct unknown_ref : weak_ref<T> {
		using size_type = weak_ref<T>::size_type;
		using type = weak_ref<T>::type;
		protected:
			bool _strength;
			void _unknown_decrease() {
				if (get_strength()) {
					this->_strong_decrease();
				}
			}
			void _unknown_increase() {
				if (get_strength()) {
					if (this->expired()) {
						throw bad_weak_ptr();
					}
					++this->_counter->strong_owners;
				}
			}
		public:
			unknown_ref(ref_pointer<type> instance = nullptr, bool strength = false) : weak_ref<T>(instance), _strength(strength) {
				if (get_strength()) {
					++this->_counter->strong_owners;
				}
			}
			unknown_ref(const unknown_ref& other, bool strength = false) : weak_ref<T>(other), _strength(strength) {
				_unknown_increase();
			}

			template<allocatable U>
			requires std::is_base_of_v<type, U> || std::is_same_v<type, U>
			unknown_ref(const weak_ref<U>& other, bool strength = false) : weak_ref<T>(other), _strength(strength) {
				_unknown_increase();
			}

			~unknown_ref() {
				_unknown_decrease();
			}

			bool get_strength() {
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



			
			unknown_ref& operator=(ref_pointer<type> instance) {
				_unknown_decrease();
				this->_instance = instance;
				if (--this->_counter->weak_owners) {
					this->_counter = cnew<ref_counter>(
						static_cast<size_type>(1),
						static_cast<size_type>(get_strength() ? 1 : 0)
					);
				}
				return *this;
			}


			
			unknown_ref& operator=(const unknown_ref& other) {
				if (this == &other) {
					return *this;
				}
				_unknown_decrease();
				this->_weak_decrease();
				this->_instance = other.get();
				++this->_counter->weak_owners;
				_unknown_increase();
				return *this;
			}

			unknown_ref& operator=(unknown_ref&& other) {
				if (this == &other) {
					return *this;
				}
				std::swap(this->_instance, other._instance);
				std::swap(this->_counter, other._counter);
				return *this;
			}

			template<allocatable U>
			requires std::is_base_of_v<type, U> || std::is_same_v<type, U>
			unknown_ref& operator=(const weak_ref<U>& other) {
				if (reinterpret_cast<weak_ref<U>*>(this) == &other) {
					return *this;
				}
				_unknown_decrease();
				this->_weak_decrease();
				this->_instance = other.get();
				++this->_counter->weak_owners;
				_unknown_increase();
				return *this;
			}
	};
}