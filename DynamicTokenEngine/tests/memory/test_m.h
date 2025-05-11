#pragma once
#include "test_memory.h"
#include "test_dynamic_stack.h"
#include "test_dynamic_array.h"
#include "test_dynamic_string.h"
inline void test_memory() {
	test_common_memory();
	test_dynamic_stack();
	test_dynamic_array();
	test_dynamic_cstring();
}