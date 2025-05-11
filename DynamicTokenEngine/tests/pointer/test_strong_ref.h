#pragma once
#include "pointer/strong_ref.hpp"
#include "pointer/unknown_ref.hpp"
#include "../data.hpp"
inline void test_strong_ref_constructor_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::strong_ref<A>* sr = new dte_utils::strong_ref<A>(dte_utils::tmalloc<A>(1));
	dte_utils::place_at(sr->get());
	delete sr;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();
}
inline void test_strong_ref_constructor_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::strong_ref<A>* sr = new dte_utils::strong_ref<A>(dte_utils::strong_ref<A>(dte_utils::tmalloc<A>(1)));
	dte_utils::place_at(sr->get());
	delete sr;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();
}
inline void test_strong_ref_constructor_2() {
	//test for getting bad_weak_ptr :)
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::strong_ref<A>* sr = new dte_utils::strong_ref<A>(dte_utils::weak_ref<A>(dte_utils::tmalloc<A>(1)));
	dte_utils::place_at(sr->get());
	delete sr;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();

}

inline void test_strong_ref() {
	test_strong_ref_constructor_0();
	test_strong_ref_constructor_1();
}