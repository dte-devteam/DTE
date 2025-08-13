#pragma once
//common float point
#ifdef _WIN64
using floatpoint = double;
#else
using floatpoint = float;
#endif
using destructor = void(void*);
using constructor = void(void*, const void*);