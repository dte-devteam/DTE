#include "tests/memory/test_m.h"
#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"

#include "memory/xmem_wrapper.hpp"

#include "core/unit.hpp"
#include "core/table.hpp"

#include "core/data_stack.hpp"

#include "core/dte_function.hpp"


#include "core/stream.hpp"


#include "core/c_function.hpp"

#include <iostream>

#include <chrono>
#include <thread>
#include <intrin.h>

#include <vector>

#include <array>

#include <functional>
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

size_t push_virt(data_stack& ds, size_t offset) {
	ds.push_virt(ds[offset]);
	return 1;
}
size_t pop(data_stack& ds, size_t offset) {
	ds.pop(ds.get_block_num() - offset);
	return 1;
}


void afk(data_stack& ds, size_t) {
	std::cout << ds.get_memory_left() << std::endl;
	std::cout << ds.get_block_num() << std::endl;
	std::cout << *static_cast<size_t*>(ds[0]) << std::endl;
}




int main(int argc, const char* argv[]) {

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

	unit uu1{ dynamic_cstring("ABC")};
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
	ttt._t_units.emplace_back(uu0, "UU0");
	ttt._t_units.emplace_back(dynamic_cstring(":)"), "UU1");
	ttt._t_units.emplace_back(unit(), dynamic_cstring(":|"));
	std::cout << ttt.get_by_index(0).u.get_int()[0] << std::endl;
	std::cout << ttt.get_by_index(1).u.get_cstr().begin() << std::endl;
	std::cout << ttt.get_by_index(2).u.get_type() << std::endl;
	//std::cout << ttt[":("].u.get_type() << std::endl;
	std::cout << ttt.get_as_target(":(").name.begin() << std::endl;

	data_stack ds0(4096);
	*static_cast<size_t*>(ds0.push_real(sizeof(size_t))) = 10;
	ds0.push_virt(nullptr);



	std::thread thr(afk, std::move(ds0), 0);
	thr.join();
	//afk(ds, 0);


	




	data_stack ds(1000);
	*static_cast<size_t*>(ds.push_real(sizeof(size_t))) = 10;
	*static_cast<size_t*>(ds.push_real(sizeof(size_t))) = 20;
	adder(ds, 0);

	std::cout << *static_cast<size_t*>(ds[0]) << std::endl;

	std::cout << "-----------------" << std::endl;

	

	stream str{ data_stack(100), {} };
	dte_function dtef;
	strong_ref<c_function> cfsr{ 
		cnew<c_function>(adder_u, c_function::metadata{})
	};



	dtef._steps = {
		{unit{static_array<ptrdiff_t, 3>{1, 2, 3}}, 0, 1, false},
		{unit{static_array<ptrdiff_t, 3>{10, 20, 30}}, 0, 1, false},
		{{cfsr}, 0, 0, true}
	};
	dtef(str, 0);
	std::cout << str.stack.get_block_num() << std::endl;
	std::cout << str.stack.get_memory_left() << std::endl;
	for (const ptrdiff_t& i : static_cast<unit*>(str.stack[0])->get_int()) {
		std::cout << i << std::endl;
	}

	std::cin.get();





	return 0;
}
struct F
{
	~F() {
		std::cout << "~F()\n";
	}
};
