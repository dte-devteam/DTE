#pragma once
#include "pointer/weak_ref.hpp"
#include "../data.hpp"
inline void test_weak_ref_constructor_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	size_t var = 10000;
	reset_A();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	dte_utils::weak_ref<A>* wr1 = new dte_utils::weak_ref<A>(dte_utils::tmalloc<B>(1));
	dte_utils::weak_ref<B>* wr2 = new dte_utils::weak_ref<B>(dte_utils::tmalloc<B>(1));
	dte_utils::weak_ref<A>* wr3 = new dte_utils::weak_ref<A>(*wr2);

	delete wr1;
	delete wr2;
	delete wr3;
		
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	log_A();

}
inline void test_weak_ref_constructor_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::weak_ref<A>* wr = new dte_utils::weak_ref<A>(dte_utils::tmalloc<A>(1));
	dte_utils::place_at(wr->get(), 10);
	std::cout << (*wr)->i << std::endl;
	wr->get()->~A();
	free(wr->get());
	*wr = nullptr;
	log_A();
}

inline void test_weak_ref() {
	test_weak_ref_constructor_0();
	test_weak_ref_constructor_1();
}