#pragma once
#include "pointer/strong_ref.hpp"
#include "pointer/unknown_ref.hpp"
#include "../data.hpp"
inline void test_unknown_ref_constructor_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	dte_utils::unknown_ref<A>* ur = new dte_utils::unknown_ref<A>(dte_utils::strong_ref<A>(dte_utils::tmalloc<A>(1)), true);
	dte_utils::place_at(ur->get());
	delete ur;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();
}

inline void test_unknown_ref() {
	test_unknown_ref_constructor_0();
}