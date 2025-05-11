#pragma once
#include "memory/dynamic_stack.hpp"
#include "../data.hpp"
inline void test_dynamic_stack_constructor_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	//prepare data
	int init_array[N];
	int counter = -1;
	for (int& i : init_array) {
		i = ++counter;
	}
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::dynamic_stack<A>* s = new dte_utils::dynamic_stack<A>();
	delete s;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();
}
inline void test_dynamic_stack_constructor_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	//prepare data
	int init_array[N];
	int counter = -1;
	for (int& i : init_array) {
		i = ++counter;
	}
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::dynamic_stack<A>* s = new dte_utils::dynamic_stack<A>(init_array);
	delete s;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	log_A();
}
inline void test_dynamic_stack_constructor_2() {
	std::cout << "---" << __func__ << "---" << std::endl;
	//prepare data
	int init_array[N];
	int counter = -1;
	for (int& i : init_array) {
		i = ++counter;
	}
	reset_A();
	dte_utils::dynamic_stack<A>* ss = new dte_utils::dynamic_stack<A>(init_array);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::dynamic_stack<A>* st = new dte_utils::dynamic_stack<A>(*ss);
	delete st;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	delete ss;
	log_A();
}
inline void test_dynamic_stack_constructor_3() {
	std::cout << "---" << __func__ << "---" << std::endl;
	//prepare data
	int init_array[N];
	int counter = -1;
	for (int& i : init_array) {
		i = ++counter;
	}
	reset_A();
	dte_utils::dynamic_stack<A, ballocator>* ss = new dte_utils::dynamic_stack<A, ballocator>(init_array);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::dynamic_stack<A>* st = new dte_utils::dynamic_stack<A>(*ss);
	delete st;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << "per element: " << (end - start) / N << std::endl;
	delete ss;
	log_A();
}
inline void test_dynamic_stack_data() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_stack<A>* s = new dte_utils::dynamic_stack<A>({1,2,3});
	//123
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	s->push_back(s->back());
	//1233
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	s->emplace_back(4);
	//12334
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	s->pop_back(2);
	//123
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	//now for timings
	s->resize_allocated(0);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	s->push_back(1);
	s->push_back(2);
	s->push_back(3);
	s->push_back(s->back());
	s->emplace_back(4);
	s->pop_back(2);
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_stack_operators_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_stack<A>* s = new dte_utils::dynamic_stack<A>({ 1,2,3 });
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s += *s;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *s) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_stack_operators_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_stack<A>* st = new dte_utils::dynamic_stack<A>({ 1,2,3 });
	dte_utils::dynamic_stack<A>* ss = new dte_utils::dynamic_stack<A>({ 4,5,6 });
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*st += *ss + *ss;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	for (A& i : *st) {
		std::cout << i.i << " ";
	}
	std::cout << std::endl;
	delete st;
	delete ss;
	log_A();
}
inline void test_dynamic_stack() {
	test_dynamic_stack_constructor_0();
	test_dynamic_stack_constructor_1();
	test_dynamic_stack_constructor_2();
	test_dynamic_stack_constructor_3();

	test_dynamic_stack_data();

	test_dynamic_stack_operators_0();
	test_dynamic_stack_operators_1();
}