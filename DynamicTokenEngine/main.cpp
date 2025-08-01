#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"

#include "memory/xmem_wrapper.hpp"

#include "token/unit.hpp"
#include "token/table.hpp"

#include "token/data_stack.hpp"

#include "token/dte_function.hpp"


#include "token/stream.hpp"


#include "token/c_function.hpp"

#include "atomic/locker.hpp"

#include "core/functions/file_functions.hpp"

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


size_t adder(data_stack& ds, size_t offset) {
	*static_cast<size_t*>(ds[offset]) += *static_cast<size_t*>(ds[ds.get_block_num() - 1]);
	return 1;
}
size_t adder_u(data_stack& ds, size_t offset) {
	static_cast<unit*>(ds[offset])->get_int()[0] += static_cast<unit*>(ds[ds.get_block_num() - 1])->get_int()[0];
	static_cast<unit*>(ds[offset])->get_int()[1] += static_cast<unit*>(ds[ds.get_block_num() - 1])->get_int()[1];
	static_cast<unit*>(ds[offset])->get_int()[2] += static_cast<unit*>(ds[ds.get_block_num() - 1])->get_int()[2];
	return 1;
}




void run_ptr(strong_ref<int, atomic_ref_counter>* ptr) {
	size_t i = 1000000;
	while (--i) {
		strong_ref<int, atomic_ref_counter> temp(*ptr);
	}
}



void c_str_copy(void* target, const void* arg) {
	new (target) dynamic_cstring(*static_cast<const dynamic_cstring*>(arg));
}
void c_str_destr(void* block) {
	static_cast<dynamic_cstring*>(block)->~dynamic_cstring();
}


struct ifstr_args {
	const char* _Filename;
	std::ios_base::openmode _Mode = std::ios_base::in;
	int _Prot = std::ios_base::_Default_open_prot;
};
ifstr_args* ifstr_args_i = new ifstr_args(
	"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"
);
void ifstr_create(void* target, const void* arg) {
	if (arg) {
		const ifstr_args& args = *static_cast<const ifstr_args*>(arg);
		new (target) std::ifstream(args._Filename, args._Mode, args._Prot);
	}
	else {
		new (target) std::ifstream();
	}
}
void ifstr_destr(void* block) {
	static_cast<std::ifstream*>(block)->~basic_ifstream();
}




int main(int argc, const char* argv[]) {
	std::chrono::steady_clock::time_point t1, t2;

	int* const qqq = cnew<int>(1000);
	int* const qq = cnew<int>(100);
	weak_ref<int> ptr = weak_ref<int>{ qqq };
	ptr = qq;
	std::cout << ptr.get_counter()->get_weak();
	cdelete(qqq);
	cdelete(qq);

	dynamic_cstring cs;
	cs.push_back('A');
	cs.push_back('B');
	cs.push_back('C');
	cs.push_back('D');
	std::cout << cs.substr(0, 2).begin() << std::endl;

	//test_memory();
	//test_pointer();

	strong_ref<int, atomic_ref_counter> abc(cnew<int>(0));
	std::thread th1(run_ptr, &abc);
	std::thread th2(run_ptr, &abc);
	th1.join();
	th2.join();
	std::cout << abc.get_counter()->get_weak() << ":" << abc.get_counter()->get_strong() << std::endl;


	unit uu0{ static_array<ptrdiff_t, 3>{1,2,3} };
	std::cout << uu0.get_int()[0] << " "
		<< uu0.get_int()[1] << " "
		<< uu0.get_int()[2] << std::endl;

	unit uu1{ dynamic_cstring("ABC") };
	std::cout << uu1.get_cstr().begin() << std::endl;

	unit uu2{ uu1 };
	std::cout << uu2.get_cstr().begin() << std::endl;

	uu0 = "AAA";
	uu0 = "AAA";
	std::cout << uu0.get_cstr().begin() << std::endl;

	uu1 = static_array<ptrdiff_t, 3>({ 1,2,3 });
	uu0 = std::move(uu1);

	std::cout << uu0.get_int()[0] << " "
		<< uu0.get_int()[1] << " "
		<< uu0.get_int()[2] << std::endl;

	std::cout << uu1.get_cstr().begin() << std::endl;

	uu1 = strong_ref<table>(cnew<table>());


	table ttt;
	ttt.get_table_units().emplace_back(uu0, "UU0");
	ttt.get_table_units().emplace_back(dynamic_cstring(":)"), "UU1");
	ttt.get_table_units().emplace_back(unit(), dynamic_cstring(":|"));
	std::cout << ttt.get_by_index(0).u.get_int()[0] << std::endl;
	std::cout << ttt.get_by_index(1).u.get_cstr().begin() << std::endl;
	std::cout << ttt.get_by_index(2).u.get_type() << std::endl;
	//std::cout << ttt[":("].u.get_type() << std::endl;
	std::cout << ttt.get_as_target(":(").name.begin() << std::endl;

	
	





	std::cout << "-----------------" << std::endl;
	stream* strf = new stream{ {10000}, {} };
	strong_ref<c_function> cfssr{
		cnew<c_function>(create_ifstream, c_function::metadata{})
	};
	strong_ref<c_function> ofsr{
		cnew<c_function>(open_file, c_function::metadata{})
	};
	strong_ref<c_function> rlsr{
		cnew<c_function>(read_line, c_function::metadata{})
	};
	strong_ref<c_function> cffsr{
		cnew<c_function>(close_file, c_function::metadata{})
	};
	dte_function dteff({ "FILE" },
		{
			//{unit{cfssr}, 0, true, {1}},
			{unit{}, 0, false, {1}, {ifstr_create, ifstr_destr, sizeof(std::ifstream), ifstr_args_i}},
			//{unit{"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"}, 1, false, {1}},
			{unit{}, 1, false, {1}, {c_str_copy, c_str_destr, sizeof(dynamic_cstring), new dynamic_cstring()}},
			//{unit{ofsr}, 0, true, {1}},
			{unit{rlsr}, 0, true, {1}, {}},
			{unit{rlsr}, 0, true, {1}, {}},
			{unit{cffsr}, 0, true, {1}, {}}
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




	std::cin.get();




	stream strd{ {100}, {} };
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 100;
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 10;
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 2;
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 2;
	strong_ref<c_function> datcf{
		cnew<c_function>(push_da_virtual, c_function::metadata{})
	};
	strong_ref<c_function> datcd{
		cnew<c_function>(cond_dec, c_function::metadata{})
	};
	dte_function datf({ "VA_ARGS" },
		{
			{unit{datcf}, 0, true, {1}},
			{unit{datcd}, 0, true, {size_t(-1),1}}
		}
	);
	t1 = std::chrono::high_resolution_clock::now();
	datf(strd, 2);
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "\n";
	std::cout << strd.stack.get_block_num() << std::endl;
	std::cout << *static_cast<size_t*>(strd.stack[4]) << std::endl;
	std::cout << *static_cast<size_t*>(strd.stack[5]) << std::endl;



	return 0;
}
