#include "CppUnitTest.h"
#include "pointer/weak_ref.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace dte_utils;
namespace dte_test::pointer::weak_reference {
	struct A {
		virtual ~A() {}
	};
	struct B : A {};
	TEST_CLASS(WEAK_REF_CONSTRUCTOR) {
		public:
			TEST_METHOD(WEAK_REF_PTR_CONSTRUCTOR) {
				int* const i = cnew<int>(1000);
				weak_ref<int> ptr = weak_ref<int>{ i };
				Assert::AreEqual(*i, *ptr);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_0) {
				int* const i = cnew<int>(1000);
				weak_ref<int> first = weak_ref<int>{ i };
				weak_ref<int> second = weak_ref<int>{ first };
				Assert::AreEqual(*first, *second);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_1) {
				int* const i = cnew<int>(1000);
				weak_ref<int> first = weak_ref<int>{ i };
				weak_ref<int> second = weak_ref<int>{ first };
				Assert::AreEqual(
					(void*)first.get_counter(), 
					(void*)second.get_counter()
				);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_2) {
				int* const i = cnew<int>(1000);
				weak_ref<int> first = weak_ref<int>{ i };
				weak_ref<int> second = weak_ref<int>{ first };
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(2),
					first.get_counter()->get_weak()
				);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_3) {
				int* const i = cnew<int>(1000);
				weak_ref<int> first = weak_ref<int>{ i };
				weak_ref<int> second = weak_ref<int>{ first };
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(0),
					first.get_counter()->get_strong()
				);
				cdelete(i);
			}
	};
	TEST_CLASS(WEAK_REF_GET) {
		public:
			TEST_METHOD(EXPIRED) {
				int* const i = cnew<int>(1000);
				weak_ref<int> ptr = weak_ref<int>{ i };
				Assert::IsTrue(ptr.expired());
				cdelete(i);
			}
	};
	TEST_CLASS(WEAK_REF_OPERATOR) {
		public:
			TEST_METHOD(PTR_OPERATOR_0) {
				int* const i = cnew<int>(1000);
				int* const ii = cnew<int>(100);
				weak_ref<int> ptr = weak_ref<int>{ i };
				const ref_counter* const RC = ptr.get_counter();
				ptr = ii;
				Assert::AreEqual(*ptr, *ii);
				Assert::AreEqual(
					(void*)ptr.get_counter(),
					(void*)RC
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(PTR_OPERATOR_1) {
				int* const i = cnew<int>(1000);
				int* const ii = cnew<int>(100);
				weak_ref<int> ptr_0 = weak_ref<int>{ i };
				weak_ref<int> ptr_1 = weak_ref<int>{ ptr_0 };
				const ref_counter* const RC = ptr_0.get_counter();
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(2),
					ptr_0.get_counter()->get_weak()
				);
				ptr_0 = ii;
				Assert::AreNotEqual(
					(void*)ptr_0.get_counter(),
					(void*)RC
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(PTR_OPERATOR_2) {
				int* const i = cnew<int>(1000);
				int* const ii = cnew<int>(100);
				weak_ref<int> ptr = weak_ref<int>{ i };
				ptr = ii;
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(0),
					ptr.get_counter()->get_strong()
				);
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(1),
					ptr.get_counter()->get_weak()
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(PTR_OPERATOR_3) {
				int* const i = cnew<int>(1000);
				int* const ii = cnew<int>(100);
				weak_ref<int> ptr = weak_ref<int>{ i };
				const ref_counter* const RC = ptr.get_counter();
				ptr = ii;
				Assert::AreEqual(
					(void*)ptr.get_counter(),
					(void*)RC
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(RVAL_OPERATOR) {
				int* const i = cnew<int>(1000);
				int* const ii = cnew<int>(100);
				weak_ref<int> ptr_0 = weak_ref<int>{ i };
				weak_ref<int> ptr_1 = weak_ref<int>{ ii };
				ptr_0 = std::move(ptr_1);
				Assert::AreEqual(*i, *ptr_1);
				Assert::AreEqual(*ii, *ptr_0);
				cdelete(i);
				cdelete(ii);
			}
	};
}