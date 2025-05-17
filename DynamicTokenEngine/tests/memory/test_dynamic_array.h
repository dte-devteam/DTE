#pragma once
#include "memory/dynamic_array.hpp"
#include "../data.hpp"
//uses stack constructors
inline void test_dynamic_array_insert_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>({ 0,1,2,3,4,5,6,7,8,9 }, 10);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->insert(s->end(), 10);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_insert_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	int arr[] = { 1,2,3,4,5 };
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>({ 0,1,2,3,4,5,6,7,8,9 });
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->insert(s->begin() + 2, 10, 3);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_insert_2() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	int arr[] = { 1,2,3,4,5 };
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>({ 0,1,2,3,4,5,6,7,8,9 });
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->insert(s->begin() + 2, arr, arr + 5);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_emplace() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	int arr[] = { 1,2,3,4,5 };
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>({ 0,1,2,3,4,5,6,7,8,9 });
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->emplace(s->begin() + 2, 10);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_erase_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>{ 0,1,2,3,4,5,6,7,8,9 };
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->erase(s->begin() + 2);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_erase_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>{ 0,1,2,3,4,5,6,7,8,9 };
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->erase(s->begin() + 2, s->begin() + 4);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_remove_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>{ 0,1,2,3,4,5,6,7,8,9 };
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->remove(s->begin() + 2);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_array_remove_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_array<A>* s = new dte_utils::dynamic_array<A>{ 0,1,2,3,4,5,6,7,8,9 };
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->remove(s->begin() + 2, s->begin() + 4);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}

inline void test_dynamic_array() {
	test_dynamic_array_insert_0();
	test_dynamic_array_insert_1();
	test_dynamic_array_insert_2();

	test_dynamic_array_emplace();

	test_dynamic_array_erase_0();
	test_dynamic_array_erase_1();

	test_dynamic_array_remove_0();
	test_dynamic_array_remove_1();
}