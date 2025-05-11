#pragma once
#include "pointer/unigue_ref.hpp"
#include "../data.hpp"
inline void test_unigue_ref_constructor_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::unique_ref<A>* r = new dte_utils::unique_ref<A>;
	delete r;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();
}
inline void test_unigue_ref_constructor_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	A* mem = dte_utils::tmalloc<A>(1);
	dte_utils::place_at(mem);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::unique_ref<A>* r = new dte_utils::unique_ref<A>(mem);
	delete r;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();
}
inline void test_unigue_ref_operator() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	A* mem1 = dte_utils::tmalloc<A>(1);
	A* mem2 = dte_utils::tmalloc<A>(1);
	dte_utils::place_at(mem1, 10);
	dte_utils::place_at(mem2, 100);
	dte_utils::unique_ref<A>* r1 = new dte_utils::unique_ref<A>(mem1);
	dte_utils::unique_ref<A>* r2 = new dte_utils::unique_ref<A>(mem2);
	std::cout << (*r1)->i << " & " << (*r2)->i << std::endl;
	*r1 = std::move(*r2);
	std::cout << (*r1)->i << " & " << (*r2)->i << std::endl;
	delete r1;
	delete r2;
	log_A();
}
inline void test_unigue_ref() {
	test_unigue_ref_constructor_0();
	test_unigue_ref_constructor_1();

	test_unigue_ref_operator();
}