#pragma once
#include "dynamic_array.hpp"
namespace dte_utils {
	template<typename T, template<typename> typename A = allocator>
	requires is_allocator_v<A, T>
	struct dynamic_string : dynamic_array<T, A> {
		using size_type = dynamic_array<T, A>::size_type;
		using type = dynamic_array<T, A>::type;
		using const_type = dynamic_array<T, A>::const_type;
		using pointer = dynamic_array<T, A>::pointer;
		using const_pointer = dynamic_array<T, A>::const_pointer;
		using dynamic_array<T, A>::dynamic_array;
		

		dynamic_string(size_t alocate_extra_size = 0) : dynamic_array<T, A>(alocate_extra_size + 1) {
			place_at(end(), static_cast<type>(0));
			++this->_used;
		}

		size_type str_len() const noexcept {
			return this->get_used() - 1;
		}
		bool empty_str() const noexcept {
			return !str_len();
		}

		void clear() {
			if constexpr (!std::is_trivially_destructible_v<type>) {
				destruct_range(begin() + 1, end());
			}
			this->_used = 1;
			this->back() = 0;
		}

		void push_back(const_type& value) {
			if (value) {
				this->back() = value;
				dynamic_stack<T, A>::push_back(0);
			}
		}
		void push_back(type&& value) {
			if (value) {
				this->back() = std::move(value);
				dynamic_stack<T, A>::push_back(0);
			}
		}
		//don`t forget - string must end by 0!
		void native_push(const_type& value) {
			dynamic_stack<T, A>::push_back(value);
		}
		//don`t forget - string must end by 0!
		void native_push(type&& value) {
			dynamic_stack<T, A>::push_back(std::move(value));
		}
		template<typename ...Args>
		void emplace_back(Args&&... args) = delete;
		void pop_back() {
			if (empty_str()) {
				throw zero_size_access();
			}
			*(end() - 2) = 0;
			--this->_used;
			if constexpr (!std::is_trivially_destructible_v<type>) {
				destuct_at(end());
			}
		}
		void pop_back(size_type num) {
			if (num > str_len()) {
				throw out_of_range();
			}
			*(end() - num - 1) = 0;
			if constexpr (!std::is_trivially_destructible_v<type>) {
				destruct_range(end() - num, end())
			}
			this->_used -= num;
		}
		//don`t forget - string must end by 0!
		void native_pop() {
			dynamic_stack<T, A>::pop_back();
		}
		//don`t forget - string must end by 0!
		void native_pop(size_type num) {
			dynamic_stack<T, A>::pop_back(num);
		}

		dynamic_string substr(size_type from, size_type to) {
			if (from > to) {
				throw invalid_range();
			}
			if (to > str_len()) {
				throw out_of_range();
			}
			dynamic_string new_str(this->begin() + from, to - from, 1);
			place_at(new_str.end(), static_cast<type>(0));
			++new_str._used;
			return new_str;
		}


		template<typename U, template<typename> typename UA>
		dynamic_string& operator +=(const dynamic_string<U, UA>& other) {
			native_pop();
			dynamic_stack<T, A>::operator+=(other);
			if (&other == reinterpret_cast<dynamic_string<U, UA>*>(this)) {
				native_push(0);
			}
			return *this;
		}
		template<typename U, size_t N>
		dynamic_string& operator +=(const U(&arr)[N]) {
			native_pop();
			dynamic_stack<T, A>::operator+=(arr);
			return *this;
		}

		template<typename U, template<typename> typename UA>
		dynamic_string operator+(const dynamic_string<U, UA>& other) const {
			dynamic_string new_str(this->begin(), this->get_used() - 1, other.get_used());
			array_to_array(new_str.end(), other.begin(), other.get_used());
			new_str._used += other.get_used();
			return new_str;
		}

		template<typename U, size_t N>
		dynamic_string operator+(const U(&arr)[N]) {
			dynamic_string new_str(this->begin(), this->get_used() - 1, N);
			array_to_array(new_str.end(), arr, N);
			new_str._used += N;
			return new_str;
		}		
	};

	typedef dynamic_string<char> dynamic_cstring;
	typedef dynamic_string<wchar_t> dynamic_wstring;
}