#pragma once
#include "dynamic_array.hpp"
namespace dte_utils {
	template<typename T, template<typename> typename A = allocator>
	struct dynamic_string : dynamic_array<T, A> {
		using array				= dynamic_array<T, A>;
		using stack				= array::stack;
		using alloc_hand		= array::alloc_hand;
		using alloc_inst		= array::alloc_inst;
		using size_type			= array::size_type;
		using type				= array::type;
		using const_type		= array::const_type;
		using iterator			= array::iterator;
		using const_iterator	= array::const_iterator;
		
		using array::dynamic_array;
		dynamic_string(size_t alocate_extra_size = 0) : array(alocate_extra_size + 1) {
			place_at(this->end(), 0);
			++this->_used;
		}

		size_type str_len() const noexcept {
			return this->get_used() - 1;
		}
		bool empty_str() const noexcept {
			return !str_len();
		}

		void clear() noexcept(std::is_nothrow_destructible_v<type>) {
			if constexpr (!std::is_trivially_destructible_v<type>) {
				destruct_range(this->begin() + 1, this->end());
			}
			this->_used = 1;
			this->back<true>() = 0;
		}

		void push_back(const_type& value) {
			if (value) {
				this->back<true>() = value;
				stack::push_back(0);
			}
		}
		void push_back(type&& value) {
			if (value) {
				this->back<true>() = std::move(value);
				stack::push_back(0);
			}
		}
		//don`t forget - string must end by 0!
		void native_push(const_type& value) {
			stack::push_back(value);
		}
		//don`t forget - string must end by 0!
		void native_push(type&& value) {
			stack::push_back(std::move(value));
		}
		template<typename ...Args>
		void emplace_back(Args&&... args) = delete;

		template<bool is_fail_safe = false>
		void pop_back()
		noexcept(std::is_nothrow_destructible_v<type> && is_fail_safe) {
			if constexpr (!is_fail_safe) {
				if (empty_str()) {
					throw zero_size_access();
				}
			}
			stack::pop_back<true>();
			this->back<true>() = 0;
		}
		template<bool is_fail_safe = false>
		void pop_back(size_type num)
		noexcept(std::is_nothrow_destructible_v<type> && is_fail_safe) {
			if constexpr (!is_fail_safe) {
				if (num > str_len()) {
					throw out_of_range();
				}
			}
			stack::pop_back<true>(num);
			this->back<true>() = 0;
		}
		//don`t forget - string must end by 0!
		template<bool is_fail_safe = false>
		void native_pop() 
		noexcept(std::is_nothrow_destructible_v<type>&& is_fail_safe) {
			stack::pop_back<is_fail_safe>();
		}
		//don`t forget - string must end by 0!
		template<bool is_fail_safe = false>
		void native_pop(size_type num)
		noexcept(std::is_nothrow_destructible_v<type>&& is_fail_safe) {
			stack::pop_back<is_fail_safe>(num);
		}

		dynamic_string substr(size_type from, size_type to) {
			if (from > to) {
				throw invalid_range();
			}
			if (to > str_len()) {
				throw out_of_range();
			}
			dynamic_string new_str(this->begin() + from, to - from, 1);
			place_at(new_str.end(), 0);
			++new_str._used;
			return new_str;
		}


		template<typename U, template<typename> typename UA>
		dynamic_string& operator +=(const dynamic_string<U, UA>& other) {
			native_pop<true>();
			dynamic_stack<T, A>::operator+=(other);
			if (&other == reinterpret_cast<dynamic_string<U, UA>*>(this)) {
				native_push(0);
			}
			return *this;
		}
		template<typename U, size_t N>
		dynamic_string& operator +=(const U(&arr)[N]) {
			native_pop<true>();
			dynamic_stack<T, A>::operator+=(arr);
			return *this;
		}

		template<typename U, template<typename> typename UA>
		dynamic_string operator+(const dynamic_string<U, UA>& other) const {
			dynamic_string new_str(this->begin(), this->get_used() - 1, other.get_used());
			array_to_array(other.begin(), other.end(), new_str.end());
			new_str._used += other.get_used();
			return new_str;
		}

		template<typename U, size_t N>
		dynamic_string operator+(const U(&arr)[N]) {
			dynamic_string new_str(this->begin(), this->get_used() - 1, N);
			array_to_array(f_iterator<const U>(arr), f_iterator<const U>(arr) + N, new_str.end());
			new_str._used += N;
			return new_str;
		}		
	};

	using dynamic_cstring = dynamic_string<char>;
	using dynamic_wstring = dynamic_string<wchar_t>;
}