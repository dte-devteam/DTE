#pragma once
#include "ref.hpp"
#include "memory/memory.hpp"
#include "pointer_base.hpp"
namespace dte_utils {
	template<typename T, typename RC = ref_counter>
	requires is_ref_counter_v<RC>
	struct weak_ref : pointer_base<T> {
		using size_type = RC::size_type;
		using type = pointer_base<T>::type;
		using pointer = pointer_base<T>::pointer;
		protected:
			RC* _counter;
			void _weak_decrease() const noexcept {
				if (!_counter->sub_weak()) {
					cdelete(_counter);
				}
			}
			void _strong_decrease() {
				if (!_counter->sub_strong()) {
					cdelete(_instance);
				}
			}
		public:
			weak_ref(pointer instance = nullptr) : pointer_base<T>(instance), _counter(cnew<RC>(static_cast<size_type>(1), static_cast<size_type>(0))) {}
			weak_ref(const weak_ref& other) noexcept : pointer_base<T>(other._instance), _counter(other._counter) {
				_counter->add_weak();
			}

			template<typename U>
			requires std::is_base_of_v<type, U>
			weak_ref(const weak_ref<U, RC>& other) noexcept : pointer_base<T>(other.operator->()), _counter(const_cast<RC*>(other.get_counter())) {
				_counter->add_weak();
			}

			~weak_ref() {
				_weak_decrease();
			}

			const RC* get_counter() const noexcept {
				return _counter;
			}
			bool expired() const noexcept {
				return !get_counter()->get_strong();
			}

			weak_ref& operator=(pointer instance) {
				_instance = instance;
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(
						static_cast<size_type>(1),
						static_cast<size_type>(0)
					);
				}
				return *this;
			}

			weak_ref& operator=(const weak_ref& other) noexcept {
				if (this == &other) {
					return *this;
				}
				_instance = other._instance;
				_weak_decrease();
				_counter = other._counter;
				_counter->add_weak();
				return *this;
			}

			//rvalue childs can convert to weak_ref&&
			//if this happens for different ref types - swap is invalid
			//typename U && requires protects
			template<typename U>
			requires std::is_same_v<weak_ref, U>
			weak_ref& operator=(U&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(_instance, other._instance);
				std::swap(_counter, other._counter);
				return *this;
			}
	};
}