#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"



#include "token/data_stack.hpp"

#include "token/dte_function.hpp"


#include "token/stream.hpp"


#include "token/c_function.hpp"

#include "core/functions/file_functions.hpp"
#include "core/functions/algebra.hpp"

#include "module/function_set.hpp"
#include "module/module_container.hpp"

#include <iostream>

#include <chrono>
#include <thread>
#include <intrin.h>

#include <vector>

#include <array>

#include <functional>
using namespace dte_utils;
using namespace dte_token;
using namespace dte_core;
using namespace dte_module;
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

#include <bit>
#include <memory>



enum function_index : size_t {
	CREATE_IFSTREAM,
	CREATE_CSTRING,
	READ_LINE,
	CLOSE_FILE
};
constexpr c_function fff[] = {
	{ create_ifstream, {}, ifstr_args_destructor },
	{ create_cstr, {}, dynamic_cstring_destructor },
	{ read_line, {} },
	{ close_file, {} }
};
constexpr function_set fs(fff);

dynamic_stack<atomic_weak_ref<const c_function>> ggg; // (cfh.begin(), cfh.get_num(), 0);
size_t log_int(data_stack& ds, const semi_pointer::data& spd) {
	std::cout << *get<int>(ds, spd.offset) << std::endl;
	return 0;
}


int main(int argc, const char* argv[]) {
	std::chrono::steady_clock::time_point t1, t2;
	//test_memory();
	//test_pointer();
	std::cout << fs.get_num() << std::endl;

	for (const c_function& cf : fff) {
		ggg.emplace_back(&cf);
	}

	ifstr_args* ifstr_args_i = cnew<ifstr_args>(
		"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"
	);
	std::cout << "-----------------" << std::endl;
	stream* strf = new stream{ {10000}, {} };
	dte_function dteff({ "FILE", 0 },
		{
			{ggg[CREATE_IFSTREAM],	{1}, {ifstr_args_i}},	//create new function by "registry"
			{ggg[CREATE_CSTRING],	{1}, {size_t(0)}},
			{ggg[READ_LINE],	{1}, {size_t(0)}},
			{ggg[CLOSE_FILE],	{1}, {size_t(0)}}
		}
	);
	t1 = std::chrono::high_resolution_clock::now();
	dteff(*strf, 0);
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "\n";
	std::cout << "-----------------" << std::endl;
	std::cout << strf->stack.get_block_num() << std::endl;
	std::cout << strf->stack.get_memory_left() << std::endl;
	//std::cout << static_cast<dte_token::unit*>(strf->stack[1])->get_cstr().begin() << std::endl;
	std::cout << static_cast<dynamic_cstring*>(strf->stack[1])->begin() << std::endl;
	delete strf;
	std::cout << "-----------------" << std::endl;
	strf = new stream{ {10000}, {} };
	*static_cast<int*>(strf->stack.push_real(sizeof(int), alignof(int), nullptr)) = 100;
	*static_cast<int*>(strf->stack.push_real(sizeof(int), alignof(int), nullptr)) = 10;
	//c_func_unit adder(add<int>, c_function::metadata{});
	c_function cfff(add<int>, {});
	c_function cfffl(log_int, {});
	atomic_strong_ref<dte_function> r0(cnew<dte_function>(
		dte_function::metadata{ "FILE", 0 },
		dynamic_array<dte_function::step>{
		dte_function::step{
			atomic_weak_ref<const c_function>(&cfff),
			dynamic_array<size_t>{1},
			semi_pointer(size_t(0))
		}
	}
	));
	atomic_strong_ref<dte_function> r1(cnew<dte_function>(
		dte_function::metadata{ "FILE", 0 },
		dynamic_array<dte_function::step>{
			dte_function::step{ 
				atomic_weak_ref<const c_function>(&cfffl), 
				dynamic_array<size_t>{1}, 
				semi_pointer(size_t(0))
			}
		}
	));
	dte_function add_and_log({ "FILE", 0 },
		{
			{r0, {1}, {size_t(0)}},
			{r1, {1}, {size_t(0)}}
		}
	);
	add_and_log(*strf, 0);
	//std::cout << strf->stack.get_block_num() << std::endl;
	//std::cout << strf->stack.get_memory_left() << std::endl;
	//std::cout << *static_cast<int*>(strf->stack[0]) << std::endl;
	std::cin.get();
	return 0;
}
