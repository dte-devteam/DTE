#pragma once
#include "alloc_handler.hpp"
#include "exceptions/logic_exception.hpp"
#include <initializer_list>
namespace dte_utils {
	template<typename T, template<typename> typename A = allocator>
	requires is_allocator_v<A, T>
	struct dynamic_stack : alloc_handler<T, A> {
		using size_type = alloc_handler<T, A>::size_type;
		using type = alloc_handler<T, A>::type;
		using const_type = alloc_handler<T, A>::const_type;
		using pointer = alloc_handler<T, A>::pointer;
		using const_pointer = alloc_handler<T, A>::const_pointer;
		protected:
			size_type _used;
			size_type _extend_by_el() const {
				return this->get_allocated() * 2 + 1;
			}
			A<T> _provide_buffer() const {
				A<T> new_allocator(this->get_allocated());
				array_to_array(static_cast<pointer>(new_allocator), begin(), get_used());
				return new_allocator;
			}
		public:
			dynamic_stack(size_type alocate_size = 0) : _used(0), alloc_handler<T, A>(alocate_size) {}
			template<typename U, size_type N>
			dynamic_stack(const U(&arr)[N], size_type reserved_size = 0) : dynamic_stack(arr, N, reserved_size) {}
			template<typename U>
			dynamic_stack(const U* array, size_type used_size, size_type reserved_size) : _used(used_size), alloc_handler<T, A>(used_size + reserved_size) {
				array_to_array(begin(), array, get_used());
			}
			dynamic_stack(std::initializer_list<type> il, size_type reserved_size = 0) : dynamic_stack(il.begin(), il.size(), reserved_size) {}
			dynamic_stack(const dynamic_stack& other) : _used(other.get_used()), alloc_handler<T, A>(other.get_allocated()) {
				array_to_array(begin(), other.begin(), get_used());
			}
			dynamic_stack(dynamic_stack&& other) noexcept : _used(other.get_used()), alloc_handler<T, A>(std::move(other)) {
				other._used = 0;
			}
			
			template<typename U, template<typename> typename UA>
			dynamic_stack(const dynamic_stack<U, UA>& other) : _used(other.get_used()), alloc_handler<T, A>(other.get_allocated()) {
				array_to_array(begin(), other.begin(), get_used());
			}

			~dynamic_stack() {
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(begin(), end());
				}
			}

			pointer begin() {
				return static_cast<pointer>(this->_allocator);
			}
			const_pointer begin() const {
				return static_cast<const_pointer>(this->_allocator);
			}
			pointer end() {
				return static_cast<pointer>(this->_allocator) + get_used();
			}
			const_pointer end() const {
				return static_cast<const_pointer>(this->_allocator) + get_used();
			}


			type& front() {
				if (!get_used()) {
					throw zero_size_access();
				}
				return *begin();
			}
			const_type& front() const {
				if (!get_used()) {
					throw zero_size_access();
				}
				return *begin();
			}
			type& back() {
				if (!get_used()) {
					throw zero_size_access();
				}
				return *(end() - 1);
			}
			const_type& back() const {
				if (!get_used()) {
					throw zero_size_access();
				}
				return *(end() - 1);
			}


			type& operator[](size_type index) {
				if (!(index < get_used())) {
					throw out_of_range();
				}
				return this->_allocator[index];
			}
			const_type& operator[](size_type index) const {
				if (!(index < get_used())) {
					throw out_of_range();
				}
				return this->_allocator[index];
			}


			template<typename P>
			pointer find(P predicate) {
				pointer i = end();
				while (i != begin()) {
					if (predicate(*--i)) {
						return i;
					}
				}
				return nullptr;
			}
			template<typename P>
			pointer find(P predicate) const {
				const_pointer i = end();
				while (i != begin()) {
					if (predicate(*--i)) {
						return i;
					}
				}
				return nullptr;
			}
			template<typename P>
			pointer find_ranged(P predicate, size_type from, size_type to) {
				if (from > to) {
					throw invalid_range();
				}
				if (to > get_used()) {
					throw out_of_range();
				}
				pointer i = begin() + to;
				pointer s = begin() + from;
				while (i != s) {
					if (predicate(*--i)) {
						return i;
					}
				}
				return nullptr;
			}
			template<typename P>
			const_pointer find_ranged(P predicate, size_type from, size_type to) const {
				if (from > to) {
					throw invalid_range();
				}
				if (to > get_used()) {
					throw out_of_range();
				}
				const_pointer i = begin() + to;
				const_pointer s = begin() + from;
				while (i != s) {
					if (predicate(*--i)) {
						return i;
					}
				}
				return nullptr;
			}



			size_type get_used() const {
				return _used;
			}


			void resize_allocated(size_type size) {
				if (get_used() > size) {
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin() + size, end());
					}
					_used = size;
				}
				if (this->get_allocated() != size) {
					this->_allocated = size;
					if constexpr (std::is_trivial_v<type>) {
						this->_allocator.resize(size);
					}
					else {
						A<T> new_allocator = _provide_buffer();
						if constexpr (!std::is_trivially_destructible_v<T>) {
							destruct_range(begin(), end());
						}
						this->_allocator = std::move(new_allocator);
					}
				}
			}
			void clear() {
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(begin(), end());
				}
				_used = 0;
			}

			
			template<typename U>
			void push_back(const U& value) {
				if (this->get_allocated() == get_used()) {
					//push reallocated
					this->_allocated = _extend_by_el();
					A<T> new_allocator = _provide_buffer();
					this->_allocator = std::move(new_allocator);
					place_at(end(), value);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(static_cast<pointer>(new_allocator), static_cast<pointer>(new_allocator) + get_used());
					}
				}
				else {
					//push unused
					place_at(end(), value);
				}
				++_used;
			}
			template<typename U>
			void push_back(U&& value) {
				if (this->get_allocated() == get_used()) {
					//push reallocated
					this->_allocated = _extend_by_el();
					A<T> new_allocator = _provide_buffer();
					this->_allocator = std::move(new_allocator);
					place_at(end(), std::move(value));
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(static_cast<pointer>(new_allocator), static_cast<pointer>(new_allocator) + get_used());
					}
				}
				else {
					//push unused
					place_at(end(), std::move(value));
				}
				++_used;
			}
			template<typename ...Args>
			void emplace_back(Args&&... args) {
				if (this->get_allocated() == get_used()) {
					//emplace reallocated
					this->_allocated = _extend_by_el();
					A<T> new_allocator = _provide_buffer();
					this->_allocator = std::move(new_allocator);
					place_at(end(), std::forward<Args>(args)...);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(static_cast<pointer>(new_allocator), static_cast<pointer>(new_allocator) + get_used());
					}
				}
				else {
					//emplace unused
					place_at(end(), std::forward<Args>(args)...);
				}
				++_used;
			}
			void pop_back() {
				if (!get_used()) {
					throw zero_size_access();
				}
				--_used;
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destuct_at(end());
				}
			}
			void pop_back(size_type num) {
				if (num > get_used()) {
					throw out_of_range();
				}
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(end() - num, end());
				}
				_used -= num;
			}

			bool operator==(const dynamic_stack& other) const 
			requires requires(type u) { u != u; } {
				if (get_used() != other.get_used()) {
					return false;
				}
				const_pointer this_val = begin();
				for (const_type& val : other) {
					if (val != *this_val) {
						return false;
					}
					++this_val;
				}
				return true;
			}
			bool operator!=(const dynamic_stack& other) const
			requires requires(type u) { u != u; } {
				return !(*this == other);
			}


			//assing operators
			dynamic_stack& operator =(const dynamic_stack& other) {
				if (this == &other) {
					return *this;
				}
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(begin(), end());
				}
				if (other.get_used() > this->get_allocated()) {
					this->_allocated = other.get_used();
					A<T> new_allocator(other.get_used());
					this->_allocator = std::move(new_allocator);
				}
				array_to_array(begin(), other.begin(), other.get_used());
				_used = other.get_used();
				return *this;
			}
			dynamic_stack& operator =(dynamic_stack&& other) noexcept {
				if (this == &other) {
					return *this;
				}
				std::swap(this->_allocator, other._allocator);
				std::swap(this->_allocated, other._allocated);
				std::swap(_used, other._used);
				return *this;
			}


			template<typename U, template<typename> typename UA>
			dynamic_stack& operator =(const dynamic_stack<U, UA>& other) {
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(begin(), end());
				}
				if (other.get_used() > this->get_allocated()) {
					this->_allocated = other.get_used();
					A<T> new_allocator(other.get_used());
					this->_allocator = std::move(new_allocator);
				}
				array_to_array(begin(), other.begin(), other.get_used());
				_used = other.get_used();
				return *this;
			}


			dynamic_stack& operator =(std::initializer_list<type> il) {
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(begin(), end());
				}
				if (il.size() > this->get_allocated()) {
					this->_allocated = il.size();
					A<T> new_allocator(il.size());
					this->_allocator = std::move(new_allocator);
				}
				array_to_array(begin(), il.begin(), il.size());
				_used = il.size();
				return *this;
			}
			template<typename U, size_type N>
			dynamic_stack& operator =(const U(&arr)[N]) {
				if constexpr (!std::is_trivially_destructible_v<type>) {
					destruct_range(begin(), end());
				}
				if (N > this->get_allocated()) {
					this->_allocated = N;
					A<T> new_allocator(N);
					this->_allocator = std::move(new_allocator);
				}
				array_to_array(begin(), arr, N);
				_used = N;
				return *this;
			}


			template<typename U, template<typename> typename UA>
			dynamic_stack& operator +=(const dynamic_stack<U, UA>& other) {
				if (get_used() + other.get_used() > this->get_allocated()) {
					this->_allocated = get_used() + other.get_used();
					A<T> new_allocator = _provide_buffer();
					array_to_array(static_cast<pointer>(new_allocator) + get_used(), other.begin(), other.get_used());
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					//copy to unused
					array_to_array(end(), other.begin(), other.get_used());
				}
				_used += other.get_used();
				return *this;
			}



			dynamic_stack& operator +=(std::initializer_list<type> il) {
				if (get_used() + il.size() > this->get_allocated()) {
					this->_allocated = get_used() + il.size();
					A<T> new_allocator = _provide_buffer();
					array_to_array(static_cast<pointer>(new_allocator) + get_used(), il.begin(), il.size());
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					array_to_array(end(), il.begin(), il.size());
				}
				_used += il.size();
				return *this;
			}
			template<typename U, size_type N>
			dynamic_stack& operator +=(const U(&arr)[N]) {
				if (get_used() + N > this->get_allocated()) {
					this->_allocated = get_used() + N;
					A<T> new_allocator = _provide_buffer();
					array_to_array(static_cast<pointer>(new_allocator) + get_used(), arr, N);
					if constexpr (!std::is_trivially_destructible_v<type>) {
						destruct_range(begin(), end());
					}
					this->_allocator = std::move(new_allocator);
				}
				else {
					array_to_array(end(), arr, N);
				}
				_used += N;
				return *this;
			}

			
			template<typename U, template<typename> typename UA>
			dynamic_stack operator +(const dynamic_stack<U, UA>& other) {
				dynamic_stack new_stack(begin(), get_used(), other.get_used());
				array_to_array(new_stack.end(), other.begin(), other.get_used());
				new_stack._used += other.get_used();
				return new_stack;
			}

			template<typename U, size_type N>
			dynamic_stack operator +(const U(&arr)[N]) {
				dynamic_stack new_stack(begin(), get_used(), N);
				array_to_array(new_stack.end(), arr, N);
				new_stack._used += N;
				return new_stack;
			}
	};
}