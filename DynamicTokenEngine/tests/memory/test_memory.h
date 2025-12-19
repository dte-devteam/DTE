#pragma once
#include "memory/memory.hpp"
#include "../data.hpp"
inline void test_common_memory_copy() {
	std::cout << "---" << __func__ << "---" << std::endl;
	//prepare data
	int init_array[N];
	int counter = -1;
	for (int& i : init_array) {
		i = ++counter;
	}
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::f_iterator<A> A_mem = dte_utils::tmalloc<A>(N);
	dte_utils::copy_range(dte_utils::f_iterator<int>(init_array), dte_utils::f_iterator<int>(init_array) + N, A_mem);
	dte_utils::destruct_range(A_mem, A_mem + N);
	free(A_mem.operator dte_utils::f_iterator<A>::pointer());
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}
inline void test_common_memory_construct() {
	std::cout << "---" << __func__ << "---" << std::endl;
	//prepare data
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	A* A_mem = dte_utils::tmalloc<A>(N);
	dte_utils::construct_range(dte_utils::f_iterator<A>(A_mem), dte_utils::f_iterator<A>(A_mem) + N, 0);
	dte_utils::destruct_range(dte_utils::f_iterator<A>(A_mem), dte_utils::f_iterator<A>(A_mem) + N);
	free(A_mem);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}
inline void test_common_memory_move() {
	std::cout << "---" << __func__ << "---" << std::endl;
	A init_array[N];
	int counter = -1;
	for (A& i : init_array) {
		i.i = ++counter;
	}
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::f_iterator<A> A_mem = dte_utils::tmalloc<A>(N);
	dte_utils::move_range(dte_utils::f_iterator<A>(init_array), dte_utils::f_iterator<A>(init_array) + N, A_mem);
	dte_utils::destruct_range(A_mem, A_mem + N);
	free(static_cast<A*>(A_mem));
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}
inline void test_common_memory_copy_mem() {
	std::cout << "---" << __func__ << "---" << std::endl;
	A init_array[N];
	int counter = -1;
	for (A& i : init_array) {
		i.i = ++counter;
	}
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	A* A_mem = dte_utils::tmalloc<A>(N);
	dte_utils::copy_memory(A_mem, init_array, N * sizeof(A));
	free(A_mem);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}
inline void test_common_memory_place() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::f_iterator<A> A_mem = dte_utils::tmalloc<A>(N);
	int counter = N;
	while (counter) {
		dte_utils::place_at(A_mem + counter, --counter);
	}
	dte_utils::destruct_range(A_mem, A_mem + N);
	free(A_mem.operator f_iterator<A>::pointer());
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}
inline void test_common_memory_array() {
	std::cout << "---" << __func__ << "---" << std::endl;
	int init_array[N];
	int counter = -1;
	for (int& i : init_array) {
		i = ++counter;
	}
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::f_iterator<A> A_mem = dte_utils::tmalloc<A>(N);
	dte_utils::array_to_array(dte_utils::f_iterator<int>(init_array), dte_utils::f_iterator<int>(init_array) + N, A_mem);
	dte_utils::destruct_range(A_mem, A_mem + N);
	free(A_mem.operator f_iterator<A>::pointer());
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}

inline void test_common_memory() {
	test_common_memory_copy();
	test_common_memory_construct();
	test_common_memory_move();
	test_common_memory_copy_mem();
	test_common_memory_place();
	test_common_memory_array();
}