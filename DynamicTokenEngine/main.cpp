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
	f_iterator<int> begin() { return i; }
	f_iterator<int> end() { return begin() + s; }
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
void ref_compability() {
	//checking compability
	is_iteroid_v<f_iterator, void>;
	is_iteroid_v<b_iterator, void>;

	b_iterator<void> e;
	std::cout << (int*)(++e).operator->() << std::endl;

	pointer_base<void> q;
	q = (int*)0;

	void* qq = nullptr;
	f_iterator<int> aaa;
	f_iterator<const int> hg;

	//q = aaa;

	aaa - aaa;

	//aaa is nullptr (by constructor), so... we get nullptr_access error
	//aaa.operator*();

	pointer_base<const int> llx;
	pointer_base<const int> llz = pointer_base<const int>(pointer_base<int>());
	pointer_base<const int>(pointer_base<int>(0));
	pointer_base<int> ll;// = llx;
	iterator_base<int> gg(ll);
	b_iterator<int> bbb(aaa);
	b_iterator<int> ccc(ll);
	weak_ref<int> www;
	f_iterator<int> qw(www);
	f_iterator<const volatile int> rrr(aaa);
	bool bv;
	bv = rrr == ll;
	bv = ll == rrr;
	bv = q == aaa;
	bv = aaa != (const int*)0;
	bv = llx == (int*)0;

	f_iterator<const volatile void> vit;
	std::cout << (aaa == vit) << std::endl;

	weak_ref<const int> kk = weak_ref<const int>(www);


	remove_const_ptr(llx.operator->());
}
int main(int argc, const char* argv[]) {
	SAS(FF:: * fgh) = nullptr;
	SAS(FFF:: * fgh2) = nullptr;
	bool io = (fgh == static_cast<SAS(FF::*)>(fgh2));
	fgh2 = fgh;

	complex_pointer<SAS(FFF::*), false> cpo = &FFF::sas;
	weak_ref_no_event<SAS(FF::*), ref_counter, false> sdr = weak_ref_no_event<SAS(FF::*), ref_counter, false>(cpo);
	weak_ref_no_event<SAS(FFF::*), ref_counter, false> sdr2 = weak_ref_no_event<SAS(FFF::*), ref_counter, false>(sdr);
	weak_ref_no_event<SAS(FF::*), ref_counter, false> sdr3 = weak_ref_no_event<SAS(FF::*), ref_counter, false>(sdr2);

	raw_pointer<SAS(FFF::*)> rawd2;
	raw_pointer<SAS(FF::*)> rawd = rawd2;
	rawd == rawd2;
	rawd2 == rawd;
	pointer_base<SAS(FFF::*)> pbsas;
	complex_pointer<SAS(FF::*), false> yg = complex_pointer<SAS(FF::*), false>(rawd2);
	complex_pointer<SAS(FF::*), false> yg2 = complex_pointer<SAS(FF::*), false>(pbsas);

	raw_pointer<int*> kl = new int*(new int(100));
	kl.get_value();
	delete static_cast<int*>(kl.get_value());
	delete static_cast<int**>(kl);

	weak_ref_no_event<float(SA::*), ref_counter, false> ah = weak_ref_no_event<float(SA::*), ref_counter, false>{};
	weak_ref_no_event<float(SA::*), ref_counter, true> ad = weak_ref_no_event<float(SA::*), ref_counter, true>(ah);
	weak_ref_no_event<float(SA::*), ref_counter, true> ad2 = weak_ref_no_event<float(SA::*), ref_counter, true>{};

	weak_ref_no_event<SAS, ref_counter> flwr;
	weak_ref_no_event<SA, ref_counter> iwr = flwr;


	std::cout << ad.get_counter().get_value().get_weak() << "\n";


	raw_pointer<int> rint;
	raw_pointer<int> rint2;
	weak_ref_no_event<SA(FF::*), ref_counter, true> wrne_int222 = { nullptr, nullptr };
	weak_ref_no_event<SAS(FFF::*), ref_counter, true> wrne_int = { nullptr, nullptr };
	rint == rint2;

	//will cause error - uncomparable types
	//rint == wrne_int;
	//rint == wrne_int222;
	//if (wrne_int222 == wrne_int) {
	//	std::cout << "YES\n";
	//}

	ad = weak_ref_no_event<float(SA::*), ref_counter, false>();

	std::cout << ad.get_counter().get_value().get_weak() << "\n";

	

	g();
	ref_compability();
	//test_memory();
	//test_pointer();


	SA nigga;
	for (int u : nigga) {
		std::cout << u << " ";
	}
	std::cout << nigga.begin()[1];
	nigga.begin() + 3;
	std::cout << std::endl;

	const SA GH;

	//compiles but causes nullptr error
	/*
	int* iptr = nullptr;
	pointer_base<int*> pb(&iptr);
	pointer_base<double(SA::***)> pbp;
	pb.get_value().get_value();
	pbp.get_value().get_value();
	*/

	SAS GI;
	float(SA:: * sd) = static_cast<float(SA::*)>(&SAS::fl);

	complex_pointer<float(SAS::*), false> yyy(&SAS::fl, &GI);
	complex_pointer<float(SAS::*), false> yyy2(yyy);
	yyy.get_owner();
	yyy2() = 10;
	std::cout << yyy2() << std::endl;
	complex_pointer<int> zzz(new int);
	*zzz = 100;
	delete zzz.operator->();

	float(SA::* fi) = &SA::fl;

	f_iterator<float(SA::*)> fii(&fi);

	nigga.**fii;

	pointer_base<void(SA::*)(float) const> rwqo(&SA::fff);

	void(SA::* abc)(float) const = &SA::fff;
	pointer_base<void(SA::**)(float) const> gr(&abc);
	decltype(*gr) a = *gr;
	a = &SA::fff;
	a = rwqo.operator pointer_base<void(SA::*)(float) const>::pointer();

	rwqo(nigga, 0.F);

	pointer_base<const int*(SA::*)> rq(&SA::ptr);
	pointer_base<void(SA::*)(int)> rwq(&SA::ff);
	pointer_base<float(SA::*)> qqh(&SA::fl);

	pointer_base<int(SA::*)() const noexcept> qh(&SA::f);
	if (qh) {
		qh(nigga);
	}
	qqh(nigga) = 10;
	if (qh) {
		qh(nigga);
	}
	rwq(nigga, 100);

	qqh(GH);
	rq(GH);

	rwq(nigga, 0);

	rwq(nigga, 0);

	rwqo(GH, 0.05F);


	
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
	std::cout << static_cast<dynamic_cstring*>(strf->stack[1])->begin().operator f_iterator<char>::pointer() << std::endl;
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
