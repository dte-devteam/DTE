//#include "tests/memory/test_m.h"
//#include "tests/pointer/test_p.h"

#include "memory/static_array.hpp"



#include "token/data_stack.hpp"

#include "token/dte_function.hpp"

#include "token/dte_type.hpp"

#include "token/stream.hpp"


#include "token/c_function.hpp"

#include "core/functions/file_functions.hpp"
#include "core/functions/algebra.hpp"

#include "module/function_set.hpp"
#include "module/module_container.hpp"

#include "pointer/iterator.hpp"

#include "pointer/complex_pointer.hpp"

#include <iostream>

#include <chrono>
#include <thread>

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
constexpr raw_pointer<int> a { nullptr };
dynamic_stack<atomic_weak_ref<const c_function>> ggg; // (cfh.begin(), cfh.get_num(), 0);
size_t log_int(data_stack& ds, const semi_pointer::data& spd) {
	std::cout << *get<int>(ds, spd.offset) << std::endl;
	return 0;
}
size_t nop(data_stack& ds, const semi_pointer::data& spd) {
	return 0;
}
size_t thr(data_stack& ds, const semi_pointer::data& spd) {
	throw exception(0, "HAHA");
}
struct H
{
	int i = 1000;
};
constexpr size_t s = 10;
struct SA {
	SA() {
		i = new int[s] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	}
	virtual ~SA() {
		delete i;
	}
	float fl = 0;
	int* i;
	operator int*() {
		return i;
	}
	int f() const noexcept {
		std::cout << "UUU" << fl << std::endl;
		return 0;
	}
	void ff(int i) {
		std::cout << "OOO" << i << std::endl;
	}
	void fff(float i) const {
		std::cout << "WWW" << i << std::endl;
	}
	const int* ptr = nullptr;
};
struct SAS : SA {

};
struct FF {
	SAS sas;
};
struct FFF : H, FF {

};
void g() {
	//weak_ref<float(SAS::*), ref_counter, ref_type::EVENT_NOEXCEPT> hj;
	//weak_ref<float(SAS::*), ref_counter, ref_type::EVENT_NOEXCEPT> hj2 = weak_ref<float(SAS::*), ref_counter, ref_type::EVENT_NOEXCEPT_CONST>(hj);



	//weak_ref<float(SAS::*)> ds;
	//weak_ref<float(SAS::*), ref_counter, ref_type::EVENT_NOEXCEPT> hj2 = weak_ref<float(SAS::*), ref_counter, ref_type::EVENT_NOEXCEPT_CONST>(ds);


}
struct dout {
	~dout() {
		std::cout << "~dout()\n";
	}
};
int main(int argc, const char* argv[]) {
	weak_ref_no_event<dout, ref_counter> dout_ptr = cnew<dout>();
	weak_ref_with_event<dout, ref_counter, true> weakptr = dout_ptr;
	weak_ref_with_event<dout, ref_counter, false> weakptr2 = weakptr;
	strong_ref_no_event<dout, ref_counter> strd = cnew<dout>();
	strong_ref_no_event<dout, ref_counter, true> strd2 = strd;

	weak_ref_no_event<int, ref_counter> qwe;
	weak_ref_no_event<int, atomic_ref_counter> qwe2;
	//error :)
	//qwe = qwe2;
	//qwe = pointer_base<int>();

	float(SA:: * we) = nullptr;
	float(SAS:: * we2) = nullptr;
	//we = we2;
	//we2 = we;

	
	//TODO: Why error occures?
	//is_same_template_v<weak_ref_no_event, complex_pointer>;

	SAS(FF:: * fgh) = nullptr;
	SAS(FFF:: * fgh2) = nullptr;
	bool io = (fgh == static_cast<SAS(FF::*)>(fgh2));
	fgh2 = fgh;


	raw_pointer<SAS(FF::*)> rawd2;
	raw_pointer<SAS(FFF::*)> rawd = rawd2;


	

	g();
	//test_memory();
	//test_pointer();



	const SA GH;

	//compiles but causes nullptr error
	/*
	int* iptr = nullptr;
	pointer_base<int*> pb(&iptr);
	pointer_base<double(SA::***)> pbp;
	pb.get_value().get_value();
	pbp.get_value().get_value();
	*/
	dynamic_stack<int>::iterator zxc;
	b_iterator<float(SA::*)>::pointer bn;
	iterable_pointer<void(*)()> as = g;
	raw_pointer<iterable_pointer<void()>> jk = g;
	jk.get_value()();
	raw_pointer<const raw_pointer<const int>> jk2 = cnew<raw_pointer<const int>>(new int(100));
	jk2.get_value().get_value();
	int* xc = nullptr;
	raw_pointer<int> fv;
	fv += fv - xc;
	fv <= fv;
	raw_pointer<int> rpa, rpb;
	std::cout << std::boolalpha << (rpa > rpb) << std::endl;
	std::cout << rpa++.operator->() << std::endl;
	std::cout << rpa.operator->() << std::endl;
	++rpa;
	fv = new int(10);
	std::cout << fv[0] << std::endl;
	delete fv.operator->();
	
	std::chrono::steady_clock::time_point t1, t2;
	//test_memory();
	//test_pointer();
	std::cout << fs.get_num() << std::endl;

	for (const c_function& cf : fff) {
		ggg.emplace_back(&cf);
	}
	ifstr_args* ifstr_args_i = cnew<ifstr_args>(
		"C:\\Users\\User\\Desktop\\DynamicTokenEngine\\DTE\\bin\\README.txt"
	).operator raw_pointer<ifstr_args>::pointer();
	std::cout << "-----------------" << std::endl;
	stream* strf = new stream{ 0, {10000}, {} };
	dte_function dteff({ "FILE", 0 },
		{
			{ggg[CREATE_IFSTREAM],	{1}, {ifstr_args_i}},	//create new function by "registry"
			{ggg[CREATE_CSTRING],	{1}, {size_t(0)}},
			{ggg[READ_LINE],	{1}, {size_t(0)}},
			{ggg[CLOSE_FILE],	{1}, {size_t(0)}}
		}
	);
	t1 = std::chrono::high_resolution_clock::now();
	dteff(*strf);
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "\n";
	std::cout << "-----------------" << std::endl;
	std::cout << strf->stack.get_block_num() << std::endl;
	std::cout << strf->stack.get_memory_left() << std::endl;
	//std::cout << static_cast<dte_token::unit*>(strf->stack[1])->get_cstr().begin() << std::endl;
	//std::cout << static_cast<dynamic_cstring*>(strf->stack[1])->begin().operator f_iterator<char>::pointer() << std::endl;
	delete strf;
	std::cout << "-----------------" << std::endl;
	strf = new stream{ 0, {10000}, {} };
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
	add_and_log(*strf);


	c_function thr_c(thr, { "THROW" });
	c_function nop_c(nop, { "NOP" });
	atomic_strong_ref<dte_function> thr0(cnew<dte_function>(
		dte_function::metadata{ "NOP", 0 },
		dynamic_array<dte_function::step>{
		dte_function::step{
			atomic_weak_ref<const c_function>(&nop_c),
			dynamic_array<size_t>{1},
			semi_pointer(size_t(0))
		}
	}
	));
	atomic_strong_ref<dte_function> thr1(cnew<dte_function>(
		dte_function::metadata{ "NOP&THR", 0 },
		dynamic_array<dte_function::step>{
		dte_function::step{
			atomic_weak_ref<const c_function>(&nop_c),
			dynamic_array<size_t>{1},
			semi_pointer(size_t(0))
		},
			dte_function::step{
				atomic_weak_ref<const c_function>(&thr_c),
				dynamic_array<size_t>{1},
				semi_pointer(size_t(0))
		}
	}
	));
	dte_function thr_catch({ "CALL", 0 },
		{
			{thr0, {1}, {size_t(0)}},
			{thr1, {1}, {size_t(0)}}
		}
	);
	try {
		thr_catch(*strf);
	}
	catch (const exception& e) {
		std::cout << e.what() << std::endl;
		//log call stack----------------------------
		for (const dte_function* f : strf->call_stack) {
			std::cout << f->meta.name.begin().operator pointer_base<const char>::pointer() << std::endl;
		}
		const dte_function::step& st = strf->call_stack.back()->steps.operator[]<true>(strf->functional_index);
		if (st.get_is_dynamic()) {
			std::cout << st.get_dte_function<true>().meta.name.begin().operator pointer_base<const char>::pointer() << std::endl;
		}
		else {
			std::cout << st.get_c_function<true>().get_meta().name << std::endl;
		}
		//------------------------------------------
	}

	delete strf;

	atomic_strong_ref<dte_type> dte_t0(cnew<dte_type>());
	atomic_strong_ref<dte_type> dte_t1(cnew<dte_type>());
	//dte_t0->add_parent(dte_t1);

	//std::cout << dte_t0->is_basic_type() << std::endl;
	//std::cout << dte_t1->is_basic_type() << std::endl;

	std::cin.get();
	return 0;
}
