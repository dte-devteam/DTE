#pragma once
#include "dynamic_array.hpp"
namespace dte_utils {
	template<allocatable T, template<allocatable> typename A = allocator>
	requires is_allocator<A, T> && std::is_move_assignable_v<T> && nullable<T>
	struct dynamic_string : dynamic_array<T, A> {
		using size_type = dynamic_array<T, A>::size_type;
		using type = dynamic_array<T, A>::type;
		using const_type = dynamic_array<T, A>::const_type;
		using pointer = dynamic_array<T, A>::pointer;
		using const_pointer = dynamic_array<T, A>::const_pointer;
		using dynamic_array<T, A>::dynamic_array;
		

		dynamic_string(size_t alocate_extra_size = 0) : dynamic_array<T, A>(alocate_extra_size + 1) {
			this->push_back(0);
		}

		size_type str_len() const {
			return this->get_used() - 1;
		}
		bool empty_str() const {
			return !str_len();
		}


		dynamic_string substr(size_type from, size_type to) {
			if (from > to) {
				throw invalid_range();
			}
			if (to > str_len()) {
				throw out_of_range();
			}
			dynamic_string new_str(this->begin() + from, to - from, 0);
			new_str.push_back(0);
			return new_str;
		}


		template<copy_constructible<type> U, template<copy_constructible<type>> typename UA>
		dynamic_string& operator +=(const dynamic_string<U, UA>& other) {
			this->pop_back();
			dynamic_array<T, A>::operator+=(other);
			if (&other == reinterpret_cast<dynamic_string<U, UA>*>(this)) {
				this->push_back(0);
			}
			return *this;
		}
		template<copy_constructible<type> U, size_t N>
		dynamic_string& operator +=(const U(&arr)[N]) {
			this->pop_back();
			dynamic_array<T, A>::operator+=(arr);
			return *this;
		}

		template<copy_constructible<type> U, template<copy_constructible<type>> typename UA>
		dynamic_string operator+(const dynamic_string<U, UA>& other) const {
			dynamic_string new_str(this->begin(), this->get_used() - 1, other.get_used());
			array_to_array(new_str.end(), other.begin(), other.get_used());
			new_str._used += other.get_used();
			return new_str;
		}

		template<copy_constructible<type> U, size_t N>
		dynamic_string operator+(const U(&arr)[N]) {
			dynamic_string new_str(this->begin(), this->get_used() - 1, N);
			array_to_array(new_str.end(), arr, N);
			new_str._used += N;
			return new_str;
		}

		
	};

	using dynamic_cstring = dynamic_string<char>;
	using dynamic_wstring = dynamic_string<wchar_t>;
}