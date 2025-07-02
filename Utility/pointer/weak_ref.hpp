#pragma once
#include "ref.hpp"
#include "memory/memory.hpp"
#include "pointer_base.hpp"
namespace dte_utils {
	template<typename T>
	struct weak_ref : pointer_base<T> {
		using size_type = ref_counter::size_type;
		using type = pointer_base<T>::type;
		using pointer = pointer_base<T>::pointer;
		protected:
			ref_counter* _counter;
			void _weak_decrease() const noexcept {
				if (!--_counter->weak_owners) {
					cdelete(_counter);
				}
			}
			void _strong_decrease() {
				if (!--_counter->strong_owners) {
					cdelete(_instance);
				}
			}
		public:
			weak_ref(pointer instance = nullptr) : pointer_base<T>(instance), _counter(cnew<ref_counter>(static_cast<size_type>(1), static_cast<size_type>(0))) {}
			weak_ref(const weak_ref& other) noexcept : pointer_base<T>(other._instance), _counter(other._counter) {
				++_counter->weak_owners;
			}

			template<typename U>
			requires std::is_base_of_v<type, U>
			weak_ref(const weak_ref<U>& other) noexcept : pointer_base<T>(other.operator->()), _counter(const_cast<ref_counter*>(other.get_counter())) {
				++_counter->weak_owners;
			}

			~weak_ref() {
				_weak_decrease();
			}

			const ref_counter* get_counter() const noexcept {
				return _counter;
			}
			bool expired() const noexcept {
				return !get_counter()->strong_owners;
			}

			weak_ref& operator=(pointer instance) {
				_instance = instance;
				if (--_counter->weak_owners) {
					_counter = cnew<ref_counter>(
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
				_counter = const_cast<ref_counter*>(other.get_counter());
				++_counter->weak_owners;
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