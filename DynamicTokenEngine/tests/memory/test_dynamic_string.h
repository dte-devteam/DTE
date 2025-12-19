#pragma once
#include "../data.hpp"
#include "memory/dynamic_string.hpp"
//uses array constructors
inline void test_dynamic_cstring_constructor() {
	std::cout << "---" << __func__ << "---" << std::endl;
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABCDEF");
	dte_utils::dynamic_cstring* e = new dte_utils::dynamic_cstring();
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::cout << s->substr(0, 3).begin().operator char*() << std::endl;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;
	std::cout << e->begin().operator char*() << std::endl;
	delete s;
}
inline void test_dynamic_cstring_operator_0() {
	std::cout << "---" << __func__ << "---" << std::endl;
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABC");
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s += *s;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;

	std::cout << s->begin().operator char*() << std::endl;
	delete s;
}
inline void test_dynamic_cstring_operator_1() {
	std::cout << "---" << __func__ << "---" << std::endl;
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABC");
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s += "END";
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;

	std::cout << s->begin().operator char*() << std::endl;
	delete s;
}
inline void test_dynamic_cstring_operator_2() {
	std::cout << "---" << __func__ << "---" << std::endl;
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABC");
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s += "END";
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;

	std::cout << s->begin().operator char*() << std::endl;
	delete s;
}
inline void test_dynamic_cstring_operator_3() {
	std::cout << "---" << __func__ << "---" << std::endl;
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABC");
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s = *s + "END";
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;

	std::cout << s->begin().operator char*() << std::endl;
	delete s;
}
inline void test_dynamic_cstring_operator_4() {
	std::cout << "---" << __func__ << "---" << std::endl;
	dte_utils::dynamic_cstring* s = new dte_utils::dynamic_cstring("ABC");
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	*s = *s + *s;
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << end - start << std::endl;

	std::cout << s->begin().operator char*() << std::endl;
	delete s;
}
inline void test_dynamic_cstring() {
	test_dynamic_cstring_constructor();

	test_dynamic_cstring_operator_0();
	test_dynamic_cstring_operator_1();
	test_dynamic_cstring_operator_2();
	test_dynamic_cstring_operator_3();
	test_dynamic_cstring_operator_4();
}