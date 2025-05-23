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

	free(wr1->get());
	free(wr2->get());

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
	dte_utils::weak_ref<A>* wr1 = new dte_utils::weak_ref<A>(dte_utils::cnew<A>(10));
	dte_utils::weak_ref<A>* wr2 = new dte_utils::weak_ref<A>(dte_utils::cnew<A>(100));
	std::cout << (*wr1)->i << " & " << (*wr2)->i << std::endl;
	*wr1 = std::move(*wr2);
	std::cout << (*wr1)->i << " & " << (*wr2)->i << std::endl;
	dte_utils::cdelete(wr1->get());
	dte_utils::cdelete(wr2->get());
	*wr1 = nullptr;
	delete wr1;
	delete wr2;
	log_A();
}


inline void test_weak_ref() {
	test_weak_ref_constructor_0();
	test_weak_ref_constructor_1();
}