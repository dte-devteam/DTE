#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"

#include "core/data.h"

#include <iostream>

#include <chrono>
#include <thread>
#include <intrin.h>

#include <vector>
using namespace dte_utils;



/*
template<typename F, typename ...Args>
ull measure(F&& f, Args&&... args) {
	std::this_thread::yield();

	__asm xor eax, eax
	__asm cpuid
	ull start_time = __rdtsc();
	
	f(std::forward<Args>(args)...);

	__asm xor eax, eax
	__asm cpuid
	return __rdtsc() - start_time;
}
*/
int main(int argc, const char* argv[]) {
	std::is_assignable_v<A, A&&>;
	std::is_constructible_v<int[3], A&&>;
	//std::cout << dte_utils::nullable<std::vector<int>> << std::endl;
	//std::cout << allocatable<unit> << std::endl;
	std::is_assignable_v<A, double>;
	char aaa[] = { 'A', 'B' };
	static_array<static_array<char, 2>, 3> abc { 
		{L'A',L'B'},
		{'A','B'},
		{'C','\0'}
	};
	std::cout << abc[0].begin() << std::endl;

	//static_array<char, 2> aaaaa;
	//const static_array<wchar_t, 2> bbbbb{ L'A', L'A' };
	//aaaaa = bbbbb;
	
	unit uuu1 = unit(ptrdiff_t(10));
	unit uuu2 = unit(dynamic_cstring("DEF"));
	uuu1 = uuu2;
	std::cout << uuu1.get<dynamic_cstring>().begin() << std::endl;

	test_memory();
	test_pointer();

	std::cin.get();
	return 0;
}