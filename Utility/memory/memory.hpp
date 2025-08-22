#pragma once
#include "target_architecture.hpp"
#include "exceptions/memory_exception.hpp"
#include "function_traits.hpp"
#include <malloc.h>
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
	template<typename T, bool align = false>
	inline T* tmalloc(size_t num) {
		return static_cast<T*>(xmalloc(num * sizeof(T)));
	}
	inline void* aligned_xmalloc(size_t size, size_t alignment) {
		if (size) {
			const size_t offset = sizeof(void*) + alignment - 1;
			void* original = xmalloc(size + offset);
			void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) + offset) & ~(alignment - 1));
			reinterpret_cast<void**>(aligned)[-1] = original;
			return aligned;
		}
		return nullptr;
	}
	template<typename T, bool align = false>
	inline T* aligned_tmalloc(size_t num) {
		return static_cast<T*>(aligned_xmalloc(num * sizeof(T), alignof(T)));
	}
	inline void* xrealloc(void* block, size_t size) {
		if (size) {
			void* mem = realloc(block, size);
			if (mem) {
				return mem;
			}
			throw bad_realloc();
		}
		free(block);
		return nullptr;
	}
	template<typename T>
	inline T* trealloc(T* block, size_t num) {
		return static_cast<T*>(xrealloc(block, num * sizeof(T)));
	}
	inline void aligned_free(void* block) {
		if (block) {
			free(reinterpret_cast<void**>(block)[-1]);
		}
	}

	//at = invalid/nulptr -> UB
	template<typename T, typename ...Args>
	inline void place_at(T* at, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args>) {
		if constexpr (std::is_trivially_constructible_v<T, Args&&...>) {
			*at = T(std::forward<Args>(args)...);
		}
		else {
			new (at) T(std::forward<Args>(args)...);
		}
	}


	template<typename T, typename ...Args>
	inline T* cnew(Args&&... args) {
		T* ptr = aligned_tmalloc<T>(1);
		place_at(ptr, std::forward<Args>(args)...);
		return ptr;
	}

	//at = invalid/nulptr -> UB
	template<typename T>
	inline void destuct_at(T* at) noexcept(std::is_nothrow_destructible_v<T>) {
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

	//at = invalid -> UB
	//see defenition limitations: destuct_at (except for nulptr)
	template<typename T>
	inline void cdelete(T* at) noexcept(std::is_nothrow_destructible_v<T>) {
		if (at) {
			//don`t call destructor of trivial type
			if constexpr (!std::is_trivially_destructible_v<T>) {
				destuct_at(at);
			}
			//function can`t be freed
			if constexpr (!return_type_v<T>) {
				aligned_free(at);
			}
		}
	}

	//begin/end = invalid/nullptr -> UB
	//see defenition limitations: place_at (all restrictions)
	template<typename T, typename ...Args>
	inline void construct_range(T* begin, T* end, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args>) {
		while (begin != end) {
			place_at(begin, args...);
			++begin;
		}
	}
	//begin/end/dest = invalid/nullptr -> UB
	//see defenition limitations: place_at (all restrictions)
	template<typename U, typename T>
	inline void copy_range(const T* begin, const T* end, U* dest) noexcept(std::is_nothrow_constructible_v<U, const T&>) {
		while (begin != end) {
			place_at(dest, *begin);
			++dest;
			++begin;
		}
	}
	//begin/end/dest = invalid/nullptr -> UB
	//see defenition limitations: place_at (all restrictions)
	template<typename U, typename T>
	inline void move_range(T* begin, T* end, U* dest) noexcept(std::is_nothrow_constructible_v<U, T&&>) {
		while (begin != end) {
			place_at(dest, std::move(*begin));
			++dest;
			++begin;
		}
	}
	//begin/end = invalid/nullptr -> UB
	//see defenition limitations: destuct_at (all restrictions)
	template<typename T>
	inline void destruct_range(T* begin, T* end) noexcept(std::is_nothrow_destructible_v<T>) {
		while (begin != end) {
			destuct_at(--end);
		}
	}



	//Copies memory by char/CPU_WORD (count = number of bytes)
	//dest/src = invalid/nullptr -> UB
	//count = invalid -> UB
	inline void copy_memory(void* dest, const void* src, size_t count) noexcept {
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
	//Copies memory by char/CPU_WORD (count = number of bytes)
	//dest/src = invalid/nullptr -> UB
	//count = invalid -> UB
	inline void swap_memory(void* dest, void* src, size_t count) noexcept {
		if (!(
			(uintptr_t)dest % sizeof(CPU_WORD) ||
			(uintptr_t)src % sizeof(CPU_WORD) ||
			count % sizeof(CPU_WORD)
		)) {
			count /= sizeof(CPU_WORD);
			while (count) {
				std::swap(((CPU_WORD*)dest)[count], ((CPU_WORD*)src)[--count]);
			}
		}
		else {
			while (count) {
				std::swap(((char*)dest)[count], ((char*)src)[--count]);
			}
		}
	}

	//Copies array elements
	//dest/src = invalid/nullptr -> UB
	//count = invalid -> UB
	template<typename U, typename T>
	inline void array_to_array(U* dest, const T* src, size_t count) noexcept(
		(std::is_trivial_v<U> && std::is_same_v<T, U>) || std::is_nothrow_copy_constructible_v<U>
	) {
		if constexpr (std::is_trivial_v<U> && std::is_same_v<T, U>) {
			copy_memory(dest, src, count * sizeof(U));
		}
		else {
			copy_range(src, src + count, dest);
		}
	}
	//Makes an effort to copy memory, if not possible moves array elements, if not possible copies array elements
	//dest/src = invalid/nullptr -> UB
	//count = invalid -> UB
	template<typename U, typename T>
	inline void array_to_array(U* dest, T* src, size_t count) noexcept (
		(std::is_trivial_v<U> && std::is_same_v<T, U>) || std::is_constructible_v<U, T&&> ?
		std::is_nothrow_constructible_v<U, T&&> :
		std::is_nothrow_constructible_v<U, const T&>
	) {
		if constexpr (std::is_trivial_v<U> && std::is_same_v<T, U>) {
			copy_memory(dest, src, count * sizeof(U));
		}
		else if constexpr (std::is_constructible_v<U, T&&>) {
			move_range(src, src + count, dest);
		}
		else {
			copy_range(src, src + count, dest);
		}
	}
}