#pragma once
//common float point
#ifdef _WIN64
typedef double floatpoint;
#else
typedef float floatpoint;
#endif
//typedef void(*destructor)(void*);
using destructor = void(void*);
//typedef void(*constructor)(void*, const void*);
using constructor = void(void*, const void*);