#pragma once
//common float point
#ifdef _WIN64
typedef double floatpoint;
#else
typedef float floatpoint;
#endif
typedef void(*copy_constructor)(void*, void*);
typedef void(*destructor)(void*);