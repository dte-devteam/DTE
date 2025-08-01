#include "CppUnitTest.h"
#include "pointer/strong_ref.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace dte_utils;
namespace dte_test::memory::strong_reference {
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
				strong_ref<A> ptr = strong_ref<A>{ cnew<A>() };
			}
			void strong_ref_lval_constructor() {
				strong_ref<A> ptr_0 = strong_ref<A>{ cnew<A>() };
				strong_ref<A> ptr_1 = strong_ref<A>{ ptr_0 };
				Assert::AreEqual(
					(void*)ptr_0.operator->(), 
					(void*)ptr_1.operator->()
				);
			}
			void strong_ref_other_constructor() {
				strong_ref<A> ptr_0 = strong_ref<A>{ strong_ref<B>{cnew<B>()} };
			}
		public:
			TEST_METHOD(STRONG_REF_PTR_CONSTRUCTOR) {
				reset_A();
				strong_ref_ptr_constructor();
				Assert::AreEqual(A_constructed, A_destructed);
			}
			TEST_METHOD(STRONG_REF_LVAL_CONSTRUCTOR) {
				reset_A();
				strong_ref_lval_constructor();
				Assert::AreEqual(A_constructed, A_destructed);
			}
			TEST_METHOD(STRONG_REF_OTHER_CONSTRUCTOR) {
				reset_A();
				reset_B();
				Assert::AreEqual(A_constructed, A_destructed);
				Assert::AreEqual(B_constructed, B_destructed);
			}
	};
	TEST_CLASS(STRONG_REF_OPERATOR) {
		private:
			void ptr_operator() {
				strong_ref<A> ptr = strong_ref<A>{ cnew<A>() };
				A* a = cnew<A>();
				ptr = a;
				Assert::AreEqual(
					(void*)a,
					(void*)ptr.operator->()
				);
			}
			void lval_operator() {
				A* a = cnew<A>();
				strong_ref<A> ptr_0 = strong_ref<A>{};
				strong_ref<A> ptr_1 = strong_ref<A>{ a };
				ptr_0 = ptr_1;
				Assert::AreEqual(
					(void*)ptr_0.operator->(),
					(void*)ptr_1.operator->()
				);
				Assert::AreEqual(
					(void*)a,
					(void*)ptr_0.operator->()
				);
			}
			void rval_operator() {
				A* a_0 = cnew<A>();
				A* a_1 = cnew<B>();
				strong_ref<A> ptr_0 = strong_ref<A>{ a_0 };
				strong_ref<A> ptr_1 = strong_ref<A>{ a_1 };
				ptr_0 = std::move(ptr_1);
				Assert::AreEqual(
					(void*)a_1,
					(void*)ptr_0.operator->()
				);
				Assert::AreEqual(
					(void*)a_0,
					(void*)ptr_1.operator->()
				);
			}
			void weak_operator() {
				B* b = cnew<B>();
				strong_ref<A> ptr_0 = strong_ref<A>{ cnew<A>() };
				strong_ref<B> ptr_1 = strong_ref<B>{ b };
				ptr_0 = ptr_1;
				Assert::AreEqual(
					(void*)b,
					(void*)ptr_0.operator->()
				);
				Assert::AreEqual(
					static_cast<decltype(ptr_0)::size_type>(2),
					ptr_0.get_counter()->get_strong()
				);
			}
		public:
			TEST_METHOD(PTR_OPERATOR) {
				reset_A();
				ptr_operator();
				Assert::AreEqual(A_constructed, A_destructed);
			}
			TEST_METHOD(LVAL_OPERATOR) {
				reset_A();
				lval_operator();
				Assert::AreEqual(A_constructed, A_destructed);
			}
			TEST_METHOD(RVAL_OPERATOR) {
				reset_A();
				rval_operator();
				Assert::AreEqual(A_constructed, A_destructed);
			}
			TEST_METHOD(WEAK_OPERATOR) {
				reset_A();
				reset_B();
				weak_operator();
				Assert::AreEqual(A_constructed, A_destructed);
				Assert::AreEqual(B_constructed, B_destructed);
			}
	};
}