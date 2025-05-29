#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"

#include "memory/xmem_wrapper.hpp"

#include "core/unit.hpp"
#include "core/table.hpp"

#include "core/function_stack.hpp"

#include "core/function.hpp"

#include <iostream>

#include <chrono>
#include <thread>
#include <intrin.h>

#include <vector>

#include <array>
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
struct F;
void fd(void* f);
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

	test_memory();
	test_pointer();

	xmem_wrapper v(cnew<F>(), fd);


	unit uu0{ static_array<ptrdiff_t, 3>{1,2,3} };
	std::cout << uu0.get_int()[0] << " " 
		<< uu0.get_int()[1] << " " 
		<< uu0.get_int()[2] << std::endl;

	unit uu1{ dynamic_cstring("ABC")};
	std::cout << uu1.get_cstr().begin() << std::endl;

	unit uu2{ uu1 };
	std::cout << uu2.get_cstr().begin() << std::endl;

	uu0 = "AAA";
	std::cout << uu0.get_cstr().begin() << std::endl;

	uu1 = static_array<ptrdiff_t, 3>({ 1,2,3 });
	uu0 = std::move(uu1);
	
	std::cout << uu0.get_int()[0] << " "
		<< uu0.get_int()[1] << " "
		<< uu0.get_int()[2] << std::endl;

	std::cout << uu1.get_cstr().begin() << std::endl;

	std::cin.get();
	return 0;
}
struct F
{
	~F() {
		std::cout << "~F()\n";
	}
};
void fd(void* f) {
	static_cast<F*>(f)->~F();
}