#pragma once
#include "ref.hpp"
#include "memory/memory.hpp"
namespace dte_utils {
	template<allocatable T>
	struct weak_ref {
		using size_type = ref_counter::size_type;
		using type = T;
		protected:
			ref_pointer<type> _instance;
			ref_counter* _counter;
			void _weak_decrease() const {
				if (!--_counter->weak_owners) {
					free(_counter);
				}
			}
			void _strong_decrease() {
				if (!--this->_counter->strong_owners && this->_instance) {
					if constexpr (!std::is_trivially_destructible_v<type>) {
						this->_instance->~type();
					}
					free(this->_instance);
				}
			}
		public:
			weak_ref(ref_pointer<type> instance = nullptr) : _instance(instance), _counter(tmalloc<ref_counter>(1)) {
				place_at(_counter, static_cast<size_type>(1), static_cast<size_type>(0));
			}
			weak_ref(const weak_ref& other) : _instance(other._instance), _counter(other._counter)  {
				++_counter->weak_owners;
			}

			template<allocatable U>
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
			ref_pointer<type> get() const {
				return _instance;
			}

			weak_ref& operator=(ref_pointer<type> instance) {
				_instance = instance;
				if (--_counter->weak_owners) {
					_counter = tmalloc<ref_counter>(1);
					place_at(_counter, static_cast<size_type>(1), static_cast<size_type>(0));
				}
				return *this;
			}
			template<allocatable U>
			requires std::is_base_of_v<type, U> || std::is_same_v<type, U>
			weak_ref& operator=(const weak_ref<U>& other) {
				if (reinterpret_cast<weak_ref<U>*>(this) == &other) {
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
			requires !return_type_v<type> {
				if (!_instance) {
					throw nullptr_access();
				}
				return *_instance;
			}
			type* operator->() const
			requires !return_type_v<type> {
				if (!_instance) {
					throw nullptr_access();
				}
				return _instance;
			}
			template<typename R = return_type_t<type>, typename ...Args>
			R operator()(Args&&... args) const {
				return _instance(std::forward<Args>(args)...);
			}
	};
}