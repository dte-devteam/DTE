#pragma once
#include "target_architecture.hpp"
#include "exceptions/memory_exception.hpp"
#include "constraints.hpp"
#include <malloc.h>
#include <type_traits>

#include <iostream>
//TODO: some typenames cant be void/arrays
namespace dte_utils {
	inline void* xmalloc(size_t size) {
		if (size) {
			void* mem = malloc(size);
			if (mem) {
				return mem;
			}
			throw bad_malloc();
		}
		return nullptr;
	}
	template<sizeof_limits T>
	inline T* tmalloc(size_t num) {
		return static_cast<T*>(xmalloc(num * sizeof(T)));
	}
	inline void* xrealloc(void* block, size_t size) {
		void* mem = realloc(block, size);
		if (mem) {
			return mem;
		}
		throw bad_realloc();
	}
	template<sizeof_limits T>
	inline T* trealloc(T* block, size_t num) {
		return static_cast<T*>(xrealloc(block, num * sizeof(T)));
	}
	

	template<typename T, copy_constructible<T> U>
	inline void place_at(T* at, const U& value) {
		if constexpr (std::is_trivially_constructible_v<T, const U&>) {
			*at = static_cast<T>(value);
		}
		else {
			new (at) T(static_cast<T>(value));
		}
	}
	template<typename T, move_constructible<T> U>
	inline void place_at(T* at, U&& value) {
		if constexpr (std::is_trivially_constructible_v<T, U&&>) {
			*at = static_cast<T&&>(value);
		}
		else {
			new (at) T(static_cast<T&&>(value));
		}
	}
	template<typename T, typename ...Args>
	requires std::is_constructible_v<T, Args&&...>
	inline void place_at(T* at, Args&&... args) {
		if constexpr (std::is_trivially_constructible_v<T, Args&&...>) {
			*at = { std::forward<Args>(args)... };
		}
		else {
			new (at) T(std::forward<Args>(args)...);
		}
	}


	template<typename T, copy_constructible<T> U>
	inline T* cnew(const U& value) {
		T* ptr = tmalloc<T>(1);
		place_at(ptr, value);
		return ptr;
	}
	template<typename T, move_constructible<T> U>
	inline T* cnew(U&& value) {
		T* ptr = tmalloc<T>(1);
		place_at(ptr, std::move(value));
		return ptr;
	}
	template<typename T, typename ...Args>
	requires std::is_constructible_v<T, Args&&...>
	inline T* cnew(Args&&... args) {
		T* ptr = tmalloc<T>(1);
		place_at(ptr, std::forward<Args>(args)...);
		return ptr;
	}


	template<typename T>
	requires std::is_destructible_v<T>
	inline void destuct_at(T* at) {
		static_assert(!std::is_trivially_destructible_v<T>, "do not try destructing trivial data");
		if constexpr (std::is_array_v<T>) {
			for (std::remove_pointer_t<std::decay_t<T>>& elem : *at) {
				destuct_at(std::addressof(elem));
			}
		}
		else {
			at->~T();
		}
	}


	template<typename T, typename ...Args>
	requires std::is_constructible_v<T, Args&&...>
	inline void construct_range(T* begin, T* end, Args&&... args) {
		while (begin != end) {
			place_at(begin, std::forward<Args>(args)...);
			++begin;
		}
	}
	template<typename U, copy_constructible<U> T>
	inline void copy_range(const T* begin, const T* end, U* dest) {
		while (begin != end) {
			place_at(dest, static_cast<U>(*begin));
			++dest;
			++begin;
		}
	}
	template<typename U, move_constructible<U> T>
	inline void move_range(T* begin, T* end, U* dest) {
		while (begin != end) {
			place_at(dest, static_cast<U&&>(*begin));
			++dest;
			++begin;
		}
	}
	template<typename T>
	requires std::is_destructible_v<T>
	inline void destruct_range(T* begin, T* end) {
		static_assert(!std::is_trivially_destructible_v<T>, "do not try destructing trivial data");
		while (begin != end) {
			destuct_at(--end);
		}
	}
	


	//Copies memory by char/CPU_WORD (count = number of bytes)
	inline void copy_memory(void* dest, const void* src, size_t count) {
		if (!(
			(uintptr_t)dest % sizeof(CPU_WORD) ||
			(uintptr_t)src % sizeof(CPU_WORD) ||
			count % sizeof(CPU_WORD)
			)) {
			count /= sizeof(CPU_WORD);
			while (count) {
				((CPU_WORD*)dest)[count] = ((const CPU_WORD*)src)[--count];
			}
		}
		else {
			while (count) {
				((char*)dest)[count] = ((const char*)src)[--count];
			}
		}
	}



	template<typename U, copy_constructible<U> T>
	inline void array_to_array(U* dest, const T* src, size_t count) {
		if constexpr (std::is_trivial_v<U> && std::is_same_v<T, U>) {
			copy_memory(dest, src, count * sizeof(U));
		}
		else {
			copy_range(src, src + count, dest);
		}
	}
	template<typename U, copy_or_move_constructible<U> T>
	inline void array_to_array(U* dest, T* src, size_t count) {
		if constexpr (std::is_trivial_v<U> && std::is_same_v<T, U>) {
			copy_memory(dest, src, count * sizeof(U));
		}
		else if constexpr (move_constructible<T, U>) {
			move_range(src, src + count, dest);
		}
		else {
			copy_range(src, src + count, dest);
		}
	}
}