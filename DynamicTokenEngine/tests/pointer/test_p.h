#pragma once
#include "test_unigue_ref.h"
#include "test_weak_ref.h"
#include "test_strong_ref.h"
#include "test_unknown_ref.h"
inline void test_pointer() {
	test_unigue_ref();
	test_weak_ref();
	test_strong_ref();
	test_unknown_ref();
}