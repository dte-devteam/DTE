#pragma once
#include "ref_base.hpp"
namespace dte_utils {
	template<typename T, typename RC = ref_counter>
	requires is_ref_counter_v<RC>
	struct weak_ref : complex_pointer<T>, ref_base<RC> {
		using size_type	= typename RC::size_type;
		using type		= typename complex_pointer<T>::type;
		using pointer	= typename complex_pointer<T>::pointer;
		protected:
			void _strong_decrease() noexcept(std::is_nothrow_destructible_v<T>) {
				if (!_counter->sub_strong()) {
					cdelete(remove_const_ptr_base(*this));
				}
			}
		public:
			weak_ref(const complex_pointer<T>& instance = nullptr) : complex_pointer<T>(instance), ref_base<RC>(cnew<RC>(1, 0)) {}
			weak_ref(const weak_ref& other) noexcept : complex_pointer<T>(other), ref_base<RC>(other) {
				_counter->add_weak();
			}
			template<typename U>
			weak_ref(const weak_ref<U, RC>& other) noexcept
			requires(std::is_convertible_v<typename weak_ref<U, RC>::pointer, pointer>) : complex_pointer<T>(other), ref_base<RC>(remove_const_ptr_base(other.get_counter())) {
				_counter->add_weak();
			}

			weak_ref& operator=(pointer instance) {
				_instance = instance;
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(1, 0);
				}
				else {
					_counter->add_weak();
				}
				return *this;
			}

			weak_ref& operator=(const weak_ref& other) noexcept(std::is_nothrow_destructible_v<RC>) {
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
	template<typename T>
	using atomic_weak_ref = weak_ref<T, atomic_ref_counter>;
}