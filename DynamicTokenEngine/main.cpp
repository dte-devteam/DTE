#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"

#include "core/unit.hpp"
#include "core/table.hpp"

#include "core/function_stack.hpp"

#include "core/function.hpp"

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
	static_array<static_array<char, 2>, 3> abc { 
		{L'A',L'B'},
		{'A','B'},
		{'C','\0'}
	};
	std::cout << abc[0].begin() << std::endl;

	/*
	unit uuu3;
	strong_ref<table>* ttt = new strong_ref<table>(cnew<table>());
	uuu3 = *ttt;
	delete ttt;
	std::cout << uuu3.get_table_ref().get_counter()->weak_owners << std::endl;
	std::cout << uuu3.get_table_ref().get_counter()->strong_owners << std::endl;
	*/

	//test_memory();
	//test_pointer();





	


	std::cin.get();
	return 0;
}