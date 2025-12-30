#include "CppUnitTest.h"
#include "pointer/weak_ref.hpp"
#include "memory/memory.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace dte_utils;
namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<> static std::wstring ToString<pointer_base<int>>(const pointer_base<int>& ptr) { return L"pointer_base<int>"; }
	template<> static std::wstring ToString<pointer_base<const ref_counter>>(const pointer_base<const ref_counter>& ptr) { return L"pointer_base<const ref_counter>"; }

	template<> static std::wstring ToString<weak_ref<int>>(const weak_ref<int>& ptr) { return L"weak_ref<int>"; }
}
namespace dte_test::pointer::weak_reference {
	struct A {
		virtual ~A() {}
	};
	struct B : A {};
	TEST_CLASS(WEAK_REF_CONSTRUCTOR) {
		public:
			TEST_METHOD(WEAK_REF_PTR_CONSTRUCTOR) {
				const pointer_base<int> i(cnew<int>(1000));
				weak_ref<int> ptr(i);
				Assert::AreEqual(
					i.operator->(), 
					ptr.operator->(),
					L"Pointer must be saved"
				);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_0) {
				const pointer_base<int> i(cnew<int>(1000));
				weak_ref<int> first(i);
				weak_ref<int> second(first);
				Assert::AreEqual(
					first, 
					second,
					L"Pointer must be saved"
				);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_1) {
				const pointer_base<int> i(cnew<int>(1000));
				weak_ref<int> first(i);
				weak_ref<int> second(first);
				Assert::AreEqual(
					first.get_counter(),
					second.get_counter(),
					L"Same counter - same address"
				);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_2) {
				const pointer_base<int> i(cnew<int>(1000));
				weak_ref<int> first(i);
				weak_ref<int> second(first);
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(2),
					first.get_counter()->get_weak(),
					L"Must be 2 (2 weak ptr to single counter)"
				);
				cdelete(i);
			}
			TEST_METHOD(WEAK_REF_LVAL_CONSTRUCTOR_3) {
				const pointer_base<int> i(cnew<int>(1000));
				weak_ref<int> first(i);
				weak_ref<int> second(first);
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(0),
					first.get_counter()->get_strong(),
					L"Weak ref must not affect strong ref"
				);
				cdelete(i);
			}
	};
	TEST_CLASS(WEAK_REF_GET) {
		public:
			TEST_METHOD(EXPIRED) {
				const pointer_base<int> i(cnew<int>(1000));
				weak_ref<int> ptr(i);
				Assert::IsTrue(
					ptr.expired(),
					L"Weak ref can`t be strong"
				);
				cdelete(i);
			}
	};
	TEST_CLASS(WEAK_REF_OPERATOR) {
		public:
			TEST_METHOD(PTR_OPERATOR_0) {
				const pointer_base<int> i(cnew<int>(1000));
				const pointer_base<int> ii(cnew<int>(100));
				weak_ref<int> ptr(i);
				const pointer_base<const ref_counter> RC = ptr.get_counter();
				ptr = ii;
				Assert::AreEqual(
					ptr.operator->(),
					ii.operator->(),
					L"Assignment must change _instance"
				);
				Assert::AreEqual(
					ptr.get_counter(),
					RC,
					L"Assignment with 0 weak ref shouldn`t create new counter - use old one!"
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(PTR_OPERATOR_1) {
				const pointer_base<int> i(cnew<int>(1000));
				const pointer_base<int> ii(cnew<int>(100));
				weak_ref<int> first(i);
				//save first counter ptr (after assingment of int* - must change)
				const pointer_base<const ref_counter> RC = first.get_counter();
				weak_ref<int> second(first);
				first = ii;
				Assert::AreNotEqual(
					first.get_counter(),
					second.get_counter(),
					L"After assignment - it must be separated (ref counter), #first"
				);
				Assert::AreNotEqual(
					first.get_counter(),
					RC,
					L"After assignment - it must be separated (ref counter) #second"
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(PTR_OPERATOR_2) {
				const pointer_base<int> i(cnew<int>(1000));
				const pointer_base<int> ii(cnew<int>(100));
				weak_ref<int> ptr = weak_ref<int>(i);
				const pointer_base<const ref_counter> RC = ptr.get_counter();
				ptr = ii;
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(0),
					ptr.get_counter()->get_strong(),
					L"Weak ref must not affect strong ref"
				);
				Assert::AreEqual(
					static_cast<weak_ref<int>::size_type>(1),
					ptr.get_counter()->get_weak(),
					L"After assignment a pointer, ref counter must save weak refs"
				);
				Assert::AreEqual(
					ptr.get_counter(),
					RC,
					L"After assignment, counter must be saved (initial weak ref num = 1)"
				);
				cdelete(i);
				cdelete(ii);
			}
			TEST_METHOD(RVAL_OPERATOR) {
				const pointer_base<int> i(cnew<int>(1000));
				const pointer_base<int> ii(cnew<int>(100));
				weak_ref<int> first(i);
				weak_ref<int> second(ii);
				const pointer_base<const ref_counter> first_RC = first.get_counter();
				const pointer_base<const ref_counter> second_RC = second.get_counter();
				first = std::move(second);
				Assert::AreEqual(
					ii.operator->(),
					first.operator->(),
					L"After RVAL assignment pointers must be swaped (first)"
				);
				Assert::AreEqual(
					i.operator->(),
					second.operator->(),
					L"After RVAL assignment pointers must be swaped (second)"
				);
				Assert::AreEqual(
					second_RC,
					first.get_counter(),
					L"After RVAL assignment counters must be swaped (first)"
				);
				Assert::AreEqual(
					first_RC,
					second.get_counter(),
					L"After RVAL assignment counters must be swaped (second)"
				);
				cdelete(i);
				cdelete(ii);
			}
	};
}