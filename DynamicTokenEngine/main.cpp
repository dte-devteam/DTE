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
struct F;

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




std::atomic<size_t> aaaa{ size_t(1) };
void run(int r) {
	std::cout << "run start " << r << std::endl;
	size_t iii = 0;
	while (aaaa.load()) {
		aaaa.wait(aaaa.load());
	}
	++aaaa;
	std::cout << "run do " << r << std::endl;
	--aaaa;
	std::cout << "run done " << r << std::endl;
	aaaa.notify_one();
}
void run_dte(stream* s, dte_function* f) {
	(*f)(*s, 0);
}





int main(int argc, const char* argv[]) {
	std::chrono::steady_clock::time_point t1, t2;
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
			{unit{cfssr}, 0, true, {1}},
			{unit{"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"}, 1, false, {1}},
			{unit{ofsr}, 0, true, {1}},
			{unit{rlsr}, 0, true, {1}},
			{unit{cffsr}, 0, true, {1}}
		}
	);
	t1 = std::chrono::high_resolution_clock::now();
	dteff(*strf, 0);
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "\n";
	std::cout << "-----------------" << std::endl;
	std::cout << strf->stack.get_block_num() << std::endl;
	std::cout << strf->stack.get_memory_left() << std::endl;
	std::cout << static_cast<dte_token::unit*>(strf->stack[1])->get_cstr().begin() << std::endl;
	delete strf;
	std::cout << "-----------------" << std::endl;




	std::cin.get();


	/*
	stream strd{ {100}, {} };
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 100;
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 10;
	*static_cast<size_t*>(strd.stack.push_real(sizeof(size_t), nullptr)) = 2;
	strong_ref<c_function> datcf{
		cnew<c_function>(push_da_virtual, c_function::metadata{})
	};
	dte_function datf({ "FILE", 0 },
		{
			{unit{datcf}, 0, 0, true, 0}
		}
	);
	datf(strd, 2);
	std::cout << strd.stack.get_block_num() << std::endl;
	std::cout << *static_cast<size_t*>(strd.stack.back()) << std::endl;
	*/

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



	data_stack ggg2 = data_stack(std::move(data_stack(500)));
	return 0;
}
struct F
{
	~F() {
		std::cout << "~F()\n";
	}
};
