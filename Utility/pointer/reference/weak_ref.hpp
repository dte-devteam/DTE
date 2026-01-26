#pragma once
#include "ref_base.hpp"
namespace dte_utils {
	template<typename T, typename RC, bool = false>
	requires is_ref_counter_v<RC>
	struct weak_ref_no_event : complex_pointer<T>, ref_base<RC> {
		using raw_type	= typename complex_pointer<T>::raw_type;
		using type		= typename complex_pointer<T>::type;
		using pointer	= typename complex_pointer<T>::pointer;
		using owner		= typename complex_pointer<T>::owner;
		using size_type = typename ref_base<RC>::size_type;
		protected:
			//be carefull - weak must be >= strong
			template<typename U>
			weak_ref_no_event(const complex_pointer<U>& instance, size_type weak_num, size_type strong_num)
			requires(is_static_castable_v<pointer, typename complex_pointer<U>::pointer>) : complex_pointer<type>(instance), ref_base<RC>(cnew<RC>(weak_num, strong_num)) {}
			void _strong_decrease() noexcept(std::is_nothrow_destructible_v<type>) {
				if (!_counter->sub_strong()) {
					cdelete(remove_const_ptr_base(*this));
				}
			}
		public:
			weak_ref_no_event(const complex_pointer<type>& instance = {}) : weak_ref_no_event(instance, 1, 0) {}
			template<typename U>
			weak_ref_no_event(const weak_ref_no_event<U, RC>& other) noexcept
			requires(is_static_castable_v<pointer, typename weak_ref_no_event<U, RC>::pointer>) : complex_pointer<type>(other), ref_base<RC>(remove_const_ptr_base(other.get_counter())) {
				_counter->add_weak();
			}
			template<typename U, typename ORC>
			requires(!std::is_same_v<RC, ORC>)
			weak_ref_no_event(const weak_ref_no_event<U, ORC>& other) = delete;

			template<typename U>
			weak_ref_no_event& operator=(const weak_ref_no_event<type, RC>& other) {
				if (*this == other) {
					return *this;
				}
				complex_pointer<type>::operator=(other);
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(1, 0);
				}
				else {
					_counter->add_weak();
				}
				return *this;
			}

			//rvalue childs can convert to weak_ref_no_event&&
			//if this happens for different ref types - swap is invalid
			//typename U&& requires protects
			template<typename U>
			weak_ref_no_event& operator=(U&& other) 
			requires(std::is_same_v<weak_ref_no_event, U>) {
				if (this == &other) {
					return *this;
				}
				complex_pointer<type>::operator=(std::move(other));
				ref_base<RC>::operator=(std::move(other));
				return *this;
			}
	};
	template<typename C, typename V, typename RC, bool is_const>
	requires is_ref_counter_v<RC>
	struct weak_ref_no_event<V(C::*), RC, is_const> : complex_pointer<V(C::*), is_const>, ref_base<RC> {
		using raw_type	= typename complex_pointer<V(C::*), is_const>::raw_type;
		using type		= typename complex_pointer<V(C::*), is_const>::type;
		using pointer	= typename complex_pointer<V(C::*), is_const>::pointer;
		using owner		= typename complex_pointer<V(C::*), is_const>::owner;
		using size_type	= typename ref_base<RC>::size_type;
		protected:
			//be carefull - weak must be >= strong
			template<typename U>
			weak_ref_no_event(const complex_pointer<U, is_const>& instance, size_type weak_num, size_type strong_num)
			requires(is_static_castable_v<pointer, typename complex_pointer<U, is_const>::pointer>) : complex_pointer<type, is_const>(instance), ref_base<RC>(cnew<RC>(weak_num, strong_num)) {}
			void _strong_decrease() noexcept(std::is_nothrow_destructible_v<C>) {
				if (!_counter->sub_strong()) {
					cdelete(remove_const_ptr_base(_class));
				}
			}
		public:
			weak_ref_no_event(const pointer_base<type>& instance = nullptr, const pointer_base<owner>& instance_owner = nullptr) : complex_pointer<type, is_const>(instance, instance_owner), ref_base<RC>(cnew<RC>(1, 0)) {}
			template<typename U, bool other_const>
			weak_ref_no_event(const complex_pointer<U, other_const>& instance)
			requires(is_const >= other_const && is_static_castable_v<pointer, typename complex_pointer<U, other_const>::pointer>) : weak_ref_no_event(instance, 1, 0) {}
			template<typename U, bool other_const>
			weak_ref_no_event(const weak_ref_no_event<U, RC, other_const>& other) noexcept
			requires(is_const >= other_const && is_static_castable_v<pointer, typename weak_ref_no_event<U, RC, other_const>::pointer>) : complex_pointer<type, is_const>(other), ref_base<RC>(remove_const_ptr_base(other.get_counter())) {
				_counter->add_weak();
			}
			template<typename U, typename ORC, bool other_const>
			requires(is_const < other_const || !std::is_same_v<RC, ORC>)
			weak_ref_no_event(const weak_ref_no_event<U, ORC, other_const>& other) = delete;

			template<typename U, bool other_const>
			weak_ref_no_event& operator=(const weak_ref_no_event<U, RC, other_const>& other)
			requires(is_static_castable_v<pointer, typename weak_ref_no_event<U, RC, other_const>::pointer>) {
				if (*this == other) {
					return *this;
				}
				complex_pointer<type, is_const>::operator=(other);
				_class = other.get_owner();
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(1, 0);
				}
				else {
					_counter->add_weak();
				}
				return *this;
			}

			//rvalue childs can convert to weak_ref_no_event&&
			//if this happens for different ref types - swap is invalid
			//typename U&& requires protects
			template<typename U>
			weak_ref_no_event& operator=(U&& other)
				requires(std::is_same_v<weak_ref_no_event, U>) {
				if (this == &other) {
					return *this;
				}
				complex_pointer<T, is_const>::operator=(std::move(other));
				ref_base<RC>::operator=(std::move(other));
				std::swap(_class, other._class);
				return *this;
			}


			void set_owner(const pointer_base<owner>& instance_owner = nullptr) noexcept {
				if (instance_owner == _class) {
					return;
				}
				_class = instance_owner;
				if (_counter->sub_weak()) {
					_counter = cnew<RC>(1, 0);
				}
				else {
					_counter->add_weak();
				}
			}
	};

	
	





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
			//typename U&& requires protects
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