#pragma once
#include "memory/allocator.hpp"
#include <iostream>
#include <chrono>
inline int A_constructed;
inline int A_destructed;
inline void reset_A() {
	A_constructed = A_destructed = 0;
}
inline void log_A() {
	std::cout << "A constructed: " << A_constructed << std::endl;
	std::cout << "A destructed:  " << A_destructed << std::endl;
}
inline constexpr int N = 1000;
struct A {
	int i;
	A() : i(0) {
		++A_constructed;
	}
	A(int i) : i(i) {
		++A_constructed;
	}
	A(const A& other) : i(other.i) {
		++A_constructed;
	}
	A(A&& other) noexcept : i(other.i) {
		++A_constructed;
	}
	virtual ~A() {
		++A_destructed;
	}

	A& operator =(const A& a) {
		//if (&a == this) {
		//	std::cout << "SAME: " << std::endl;
		//}
		//std::cout << "A = const A&\t" << std::endl;
		i = a.i;
		return *this;
	}
	A& operator =(A&& a) noexcept {
		//if (&a == this) {
		//	std::cout << "SAME: " << std::endl;
		//}
		//std::cout << "A = A&&\t" << std::endl;
		std::swap(i, a.i);
		return *this;
	}
};
struct B : A {
	using A::A;
	~B() {}
};
template<dte_utils::sizeof_limits T>
struct ballocator : dte_utils::mem_handler {
	using size_type = mem_handler::size_type;
	using type = T;
	using const_type = const type;
	using pointer = type*;
	using const_pointer = const_type*;
	public:
		ballocator(size_type num = 0) : mem_handler(num * sizeof(T)) {}
		ballocator(const ballocator&) = delete;
		ballocator(ballocator&& other) noexcept : mem_handler(std::move(other)) {}
		void resize(size_type num) {
			static_assert(std::is_trivial_v<T>, "do not try reallocating nontrivial data");
			mem_handler::resize(num * sizeof(T));
		}
		ballocator& operator=(const ballocator&) = delete;
		ballocator& operator=(ballocator&& other) noexcept {
			if (this == &other) {
				return *this;
			}
			std::swap(_ptr, other._ptr);
			return *this;
		}
		operator pointer() {
			return static_cast<pointer>(_ptr);
		}
		operator const_pointer() const {
			return static_cast<const_pointer>(_ptr);
		}
};