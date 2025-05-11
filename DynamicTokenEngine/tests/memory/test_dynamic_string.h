#pragma once
#include "../data.hpp"
#include "memory/dynamic_string.hpp"
//uses array constructors
inline void test_dynamic_cstring_operator_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	reset_A();
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABC");
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s += *s;
	*s += "END";
	*s = *s + "NEW\n";
	*s = *s + *s;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;

	std::cout << s->begin() << std::endl;
	std::cout << s->substr(0, 3).begin() << std::endl;
	delete s;
	log_A();
}
inline void test_dynamic_cstring() {
	test_dynamic_cstring_operator_0();
}