#pragma once
#include "ref.hpp"
#include "memory/memory.hpp"
namespace dte_utils {
	template<typename T>
	struct weak_ref {
		using size_type = ref_counter::size_type;
		using type = T;
		using pointer = type*;
		protected:
			pointer _instance;
			ref_counter* _counter;
			void _weak_decrease() const {
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
			weak_ref(pointer instance = nullptr) : _instance(instance), _counter(cnew<ref_counter>(static_cast<size_type>(1), static_cast<size_type>(0))) {}
			weak_ref(const weak_ref& other) : _instance(other._instance), _counter(other._counter)  {
				++_counter->weak_owners;
			}

			template<typename U>
			requires std::is_base_of_v<type, U>
			weak_ref(const weak_ref<U>& other) : _instance(other.get()), _counter(const_cast<ref_counter*>(other.get_counter())) {
				++_counter->weak_owners;
			}

			~weak_ref() {
				_weak_decrease();
			}

			const ref_counter* get_counter() const {
				return _counter;
			}
			bool expired() const {
				return !get_counter()->strong_owners;
			}
			pointer get() const {
				return _instance;
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

			weak_ref& operator=(const weak_ref& other) {
				if (this == &other) {
					return *this;
				}
				_instance = other.get();
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


			type& operator*() const
			requires !return_type_v<pointer> {
				if (!_instance) {
					throw nullptr_access();
				}
				return *_instance;
			}
			pointer operator->() const
			requires !return_type_v<pointer> {
				if (!_instance) {
					throw nullptr_access();
				}
				return _instance;
			}
			template<typename ...Args>
			requires return_type_v<pointer>
			return_type_t<pointer> operator()(Args&&... args) const {
				return _instance(std::forward<Args>(args)...);
			}
			template<typename ...Args>
			requires is_functor_v<type, Args&&...>
			is_functor_t<type, Args&&...> operator()(Args&&... args) const {
				return _instance->operator()(std::forward<Args>(args)...);
			}
	};
}