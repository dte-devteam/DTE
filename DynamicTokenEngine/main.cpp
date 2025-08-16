#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"



#include "token/data_stack.hpp"

#include "token/dte_function.hpp"


#include "token/stream.hpp"


#include "token/c_function.hpp"

#include "core/functions/file_functions.hpp"
#include "core/functions/algebra.hpp"

#include "module/module_instance.hpp"

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
struct DDD
{
	void operator()(int) noexcept {
		std::cout << "+" << std::endl;
	}
};


int main(int argc, const char* argv[]) {
	std::chrono::steady_clock::time_point t1, t2;

	//test_memory();
	//test_pointer();

	is_functor_noexcept_v<DDD, int>;


	ifstr_args* ifstr_args_i = cnew<ifstr_args>(
		"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"
	);
	std::cout << "-----------------" << std::endl;
	stream* strf = new stream{ {10000}, {} };
	atomic_strong_ref<c_function> cfssr{
		cnew<c_function>(create_ifstream, c_function::metadata{}, ifstr_args_destructor)
	};
	atomic_strong_ref<c_function> ccstrsr{
		cnew<c_function>(create_cstr, c_function::metadata{}, dynamic_cstring_destructor)
	};
	atomic_strong_ref<c_function> rlsr{
		cnew<c_function>(read_line, c_function::metadata{})
	};
	atomic_strong_ref<c_function> cffsr{
		cnew<c_function>(close_file, c_function::metadata{})
	};
	/*
	dte_function dteff({ "FILE" },
		{
			//{unit{cfssr}, 0, true, {1}},
			{unit{}, 0, true, {1}, {ifstr_create, ifstr_destr, sizeof(std::ifstream), ifstr_args_i}, {1}},
			//{unit{"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"}, 1, false, {1}},
			{unit{}, 1, true, {1}, {c_str_copy, c_str_destr, sizeof(dynamic_cstring), new dynamic_cstring()}, {1}},
			//{unit{ofsr}, 0, true, {1}},
			{unit{rlsr}, 0, true, {1}, {}, {1}},
			{unit{rlsr}, 0, true, {1}, {}, {1}},
			{unit{cffsr}, 0, true, {1}, {}, {1}}
		}
	);*/
	dte_function dteff({ "FILE", 0 },
		{
			{cfssr, {1}, {ifstr_args_i}},
			{ccstrsr, {1}, {size_t(0)}},
			{rlsr, {1}, {size_t(0)}},
			{cffsr, {1}, {size_t(0)}}
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
	*static_cast<int*>(strf->stack.push_real(sizeof(int), nullptr)) = 100;
	*static_cast<int*>(strf->stack.push_real(sizeof(int), nullptr)) = 10;
	atomic_strong_ref<c_function> adder{
		cnew<c_function>(add<int>, c_function::metadata{})
	};
	dte_function dteaf({ "FILE", 0 },
		{
			{adder, {1}, {size_t(0)}}
		}
	);
	dteaf(*strf, 0);
	std::cout << strf->stack.get_block_num() << std::endl;
	std::cout << strf->stack.get_memory_left() << std::endl;
	std::cout << *static_cast<int*>(strf->stack[0]) << std::endl;
	std::cin.get();
	return 0;
}
