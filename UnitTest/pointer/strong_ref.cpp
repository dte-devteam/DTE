#include "CppUnitTest.h"
#include "pointer/strong_ref.hpp"
#include "memory/memory.hpp"
#include <thread>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace dte_utils;
namespace dte_test::pointer::strong_reference {
	struct A;
}
namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<> static std::wstring ToString<dte_test::pointer::strong_reference::A>(dte_test::pointer::strong_reference::A* ptr) { return L"A*"; }

	template<> static std::wstring ToString<complex_pointer<const ref_counter>>(const complex_pointer<const ref_counter>& ptr) { return L"complex_pointer<const ref_counter>"; }

	template<> static std::wstring ToString<strong_ref<dte_test::pointer::strong_reference::A>>(const strong_ref<dte_test::pointer::strong_reference::A>& ptr) { return L"strong_ref<A>"; }
}
namespace dte_test::pointer::strong_reference {
	int A_constructed;
	int A_destructed;
	int B_constructed;
	int B_destructed;
	void reset_A() {
		A_constructed = 0;
		A_destructed = 0;
	}
	void reset_B() {
		B_constructed = 0;
		B_destructed = 0;
	}
	struct A {
		A() {
			++A_constructed;
		}
		virtual ~A() {
			++A_destructed;
		}
	};
	struct B : A {
		B() {
			++B_constructed;
		}
		~B() {
			++B_destructed;
		}
	};
	TEST_CLASS(STRONG_REF_CONSTRUCTOR) {
		private:
			void strong_ref_ptr_constructor() {
				strong_ref<A> ptr(cnew<A>());
			}
			void strong_ref_lval_constructor() {
				strong_ref<A> ptr_0(cnew<A>());
				strong_ref<A> ptr_1(ptr_0);
				Assert::AreEqual(
					ptr_0,
					ptr_1,
					L"Same A* used - same pointer must be in strong refs"
				);
			}
		public:
			TEST_METHOD(STRONG_REF_PTR_CONSTRUCTOR) {
				reset_A();
				strong_ref_ptr_constructor();
				Assert::AreEqual(
					A_constructed, 
					A_destructed,
					L"Objects must be counted, destroyed, freed"
				);
			}
			TEST_METHOD(STRONG_REF_LVAL_CONSTRUCTOR) {
				reset_A();
				strong_ref_lval_constructor();
				Assert::AreEqual(
					A_constructed, 
					A_destructed,
					L"Objects must be counted, destroyed, freed"
				);
			}
	};
	//outside of class - because causes strange "&" error
	void test_atomic(atomic_strong_ref<A>* ptr) {
		size_t i = 100001;
		while (--i) {
			atomic_strong_ref<A> a{ *ptr };
		}
	}
	void test_atom_thread() {
		atomic_strong_ref<A> asra{ cnew<A>() };
		std::thread t1(test_atomic, &asra);
		std::thread t2(test_atomic, &asra);
		t1.join();
		t2.join();
	}
	//-----------------------------------------------------
	TEST_CLASS(STRONG_REF_OPERATOR) {
		private:
			void ptr_operator() {
				strong_ref<A> ptr(cnew<A>());
				const pointer_base<A> a(cnew<A>());
				ptr = a;
				Assert::AreEqual(
					a.operator->(),
					ptr.operator->(),
					L"Objects must be counted, destroyed, freed"
				);
			}
			void lval_operator() {
				const pointer_base<A> a(cnew<A>());
				strong_ref<A> ptr_0;
				strong_ref<A> ptr_1(a);
				ptr_0 = ptr_1;
				Assert::AreEqual(
					ptr_0.operator->(),
					ptr_1.operator->(),
					L"Must be same pointer (0)"
				);
				Assert::AreEqual(
					a.operator->(),
					ptr_0.operator->(),
					L"Must be same pointer (1)"
				);
			}
			void rval_operator() {
				const complex_pointer<A> a_0(cnew<A>());
				const complex_pointer<A> a_1(cnew<B>());
				strong_ref<A> ptr_0(a_0);
				strong_ref<A> ptr_1(a_1);
				const complex_pointer<const ref_counter> RC_0 = ptr_0.get_counter();
				const complex_pointer<const ref_counter> RC_1 = ptr_1.get_counter();
				ptr_0 = std::move(ptr_1);
				Assert::AreEqual(
					a_1.operator->(),
					ptr_0.operator->(),
					L"Pointers must be swapped"
				);
				Assert::AreEqual(
					a_0.operator->(),
					ptr_1.operator->(),
					L"Pointers must be swapped"
				);
				Assert::AreEqual(
					ptr_0.get_counter(),
					RC_1,
					L"Counters must be swapped"
				);
				Assert::AreEqual(
					ptr_1.get_counter(),
					RC_0,
					L"Counters must be swapped"
				);
			}
		public:
			TEST_METHOD(PTR_OPERATOR) {
				reset_A();
				ptr_operator();
				Assert::AreEqual(
					A_constructed, 
					A_destructed,
					L"Objects must be counted, destroyed, freed"
				);
			}
			TEST_METHOD(LVAL_OPERATOR) {
				reset_A();
				lval_operator();
				Assert::AreEqual(
					A_constructed, 
					A_destructed,
					L"Objects must be counted, destroyed, freed"
				);
			}
			TEST_METHOD(RVAL_OPERATOR) {
				reset_A();
				rval_operator();
				Assert::AreEqual(
					A_constructed, 
					A_destructed,
					L"Objects must be counted, destroyed, freed"
				);
			}
			TEST_METHOD(TEST_ATOM) {
				reset_A();
				test_atom_thread();
				Assert::AreEqual(
					A_constructed, 
					A_destructed,
					L"Objects must be counted, destroyed, freed"
				);
			}
	};
}