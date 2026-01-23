#pragma once
#include "target_architecture.hpp"
#include "exceptions/memory_exception.hpp"
#include "function_traits.hpp"
#include "pointer/iterator.hpp"
#include <malloc.h>
namespace dte_utils {
	template<typename T>
	concept non_void = !std::is_void_v<T>;


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
	template<non_void T>
	inline T* tmalloc(size_t num) {
		return static_cast<T*>(xmalloc(num * sizeof(T)));
	}
	template<bool is_alignment_safe = false>
	inline void* aligned_xmalloc(size_t size, size_t alignment) {
		if constexpr (!is_alignment_safe) {
			if (!alignment) {
				throw bad_alignment();
			}
		}
		if (size) {
			const size_t offset = sizeof(void*) + alignment - 1;
			void* original = xmalloc(size + offset);
			void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) + offset) & ~(alignment - 1));
			reinterpret_cast<void**>(aligned)[-1] = original;
			return aligned;
		}
		return nullptr;
	}
	template<non_void T>
	inline T* aligned_tmalloc(size_t num) {
		return static_cast<T*>(aligned_xmalloc<true>(num * sizeof(T), alignof(T)));
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

	//at = invalid -> UB
	//at = nullptr -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, bool is_fail_safe = false, typename ...Args>
	inline void place_at(const raw_pointer<T>& at, Args&&... args)
	noexcept(std::is_nothrow_constructible_v<T, Args...> && is_fail_safe) 
	requires(std::is_constructible_v<T, Args...> && !std::is_const_v<T>) {
		if constexpr (!is_fail_safe) {
			if (!at.operator raw_pointer<T>::pointer()) {
				throw nullptr_access();
			}
		}
		if constexpr (std::is_trivially_constructible_v<T, Args&&...>) {
			*(at.operator raw_pointer<T>::pointer()) = T(std::forward<Args>(args)...);
		}
		else {
			new (at.operator raw_pointer<T>::pointer()) T(std::forward<Args>(args)...);
		}
	}

	//can throw - xmalloc can throw bad_malloc
	template<typename T, typename ...Args>
	inline raw_pointer<T> cnew(Args&&... args)
	requires(std::is_constructible_v<T, Args...>) {
		raw_pointer<T> ptr(aligned_tmalloc<T>(1));
		place_at<T, true, Args...>(ptr, std::forward<Args>(args)...);
		return ptr;
	}

	template<typename T>
	inline void destuct(T& object) 
	noexcept(std::is_nothrow_destructible_v<T>)
	requires(std::is_destructible_v<T> && !std::is_const_v<T>) {
		object.~T();
	}

	//at = invalid -> UB
	//at = nullptr -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, bool is_fail_safe = false>
	inline void destuct_at(const raw_pointer<T>& at)
	noexcept(std::is_nothrow_destructible_v<T> && is_fail_safe)
	requires(std::is_destructible_v<T> && !std::is_const_v<T>) {
		if constexpr (!is_fail_safe) {
			if (!at.operator raw_pointer<T>::pointer()) {
				throw nullptr_access();
			}
		}
		if constexpr (std::is_array_v<T>) {
			for (std::remove_pointer_t<std::decay_t<T>>& elem : *(at.operator raw_pointer<T>::pointer())) {
				destuct_at(std::addressof(elem));
			}
		}
		else {
			(at.operator raw_pointer<T>::pointer())->~T();
		}
	}

	//at = invalid -> UB
	template<typename T>
	inline void cdelete(const raw_pointer<T>& at)
	noexcept(std::is_nothrow_destructible_v<T>)
	requires(std::is_destructible_v<T> && !std::is_const_v<T>) {
		if (at) {
			//don`t call destructor of trivial type
			if constexpr (!std::is_trivially_destructible_v<T>) {
				destuct_at<T, true>(at);
			}
			//function can`t be freed
			if constexpr (!return_type_v<T>) {
				aligned_free(at.operator raw_pointer<T>::pointer());
			}
		}
	}

	//begin/end = invalid -> UB
	//begin/end = nullptr -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, template<typename> typename It, bool is_fail_safe = false, typename ...Args>
	inline void construct_range(It<T> begin, const raw_pointer<T>& end, Args&&... args)
	noexcept(std::is_nothrow_constructible_v<T, Args...> && is_fail_safe)
	requires(std::is_constructible_v<T, Args...> && is_iteroid_v<It, T> && !std::is_const_v<T>) {
		//check once instead of checking in cycle
		if constexpr (!is_fail_safe) {
			if ((begin != end) && !(begin && end)) {
				throw nullptr_access();
			}
		}
		while (begin != end) {
			place_at<T, true, Args...>(begin, std::forward<Args>(args)...);
			++begin;
		}
	}
	//begin/end/dest = invalid -> UB
	//begin/end/dest = nullptr -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, typename U, template<typename> typename ItT, template<typename> typename ItU, bool is_fail_safe = false>
	inline void copy_range(ItT<T> begin, const raw_pointer<T>& end, ItU<U> dest)
	noexcept(std::is_constructible_v<U, T> && is_fail_safe)
	requires(std::is_constructible_v<U, T> && is_iteroid_v<ItT, T> && is_iteroid_v<ItU, U> && !std::is_const_v<U>) {
		//check once instead of checking in cycle
		if constexpr (!is_fail_safe) {
			if ((begin != end) && !(begin && end && dest)) {
				throw nullptr_access();
			}
		}
		while (begin != end) {
			place_at<U, true, T&>(dest, begin.operator*<true>());
			++dest;
			++begin;
		}
	}
	//begin/end/dest = invalid -> UB
	//begin/end/dest = nullptr -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, typename U, template<typename> typename ItT, template<typename> typename ItU, bool is_fail_safe = false>
	inline void move_range(ItT<T> begin, const raw_pointer<T>& end, ItU<U> dest)
	noexcept(std::is_nothrow_constructible_v<U, T&&> && is_fail_safe)
	requires(std::is_constructible_v<U, T&&> && is_iteroid_v<ItT, T> && is_iteroid_v<ItU, U> && !(std::is_const_v<T> || std::is_const_v<U>)) {
		//check once instead of checking in cycle
		if constexpr (!is_fail_safe) {
			if ((begin != end) && !(begin && end && dest)) {
				throw nullptr_access();
			}
		}
		while (begin != end) {
			place_at(dest, std::move(*begin));
			++dest;
			++begin;
		}
	}
	//begin/end = invalid -> UB
	//begin/end = nullptr -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, template<typename> typename It, bool is_fail_safe = false>
	inline void destruct_range(It<T> begin, const raw_pointer<T>& end)
	noexcept(std::is_nothrow_destructible_v<T> && is_fail_safe)
	requires(std::is_destructible_v<T> && is_iteroid_v<It, T> && !std::is_const_v<T>) {
		//check once instead of checking in cycle
		if constexpr (!is_fail_safe) {
			if ((begin != end) && !(begin && end)) {
				throw nullptr_access();
			}
		}
		while (begin != end) {
			destuct_at(begin);
			++begin;
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
	//begin/end/dest = invalid/nullptr -> UB
	//begin/end/dest -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, typename U, template<typename> typename ItT, template<typename> typename ItU, bool is_fail_safe = false>
	inline void array_to_array(const ItT<const T>& begin, const raw_pointer<const T>& end, const ItU<U>& dest)
	noexcept(std::is_nothrow_constructible_v<U, const T&> && is_fail_safe)
	requires(is_iteroid_v<ItT, T> && is_iteroid_v<ItU, U> && !std::is_const_v<U> && std::is_constructible_v<U, T> ) {
		copy_range<const T, U, ItT, ItU, is_fail_safe>(begin, end, dest);
	}
	//Moves array elements, if not possible copies array elements
	//begin/end/dest = invalid/nullptr -> UB
	//begin/end/dest -> error: nullptr_access (disable ptr checks: is_fail_safe = true)
	template<typename T, typename U, template<typename> typename ItT, template<typename> typename ItU, bool is_fail_safe = false>
	inline void array_to_array(const ItT<T>& begin, const raw_pointer<T>& end, const ItU<U>& dest)
	noexcept(std::is_constructible_v<U, T&&> ? std::is_nothrow_constructible_v<U, T&&> : std::is_nothrow_constructible_v<U, const T&> && is_fail_safe)
	requires(is_iteroid_v<ItT, T> && is_iteroid_v<ItU, U> && !std::is_const_v<U> && (std::is_constructible_v<U, T> || std::is_constructible_v<U, T&&>)) {
		if constexpr (std::is_constructible_v<U, T&&>) {
			move_range<T, U, ItT, ItU, is_fail_safe>(begin, end, dest);
		}
		else {
			copy_range<T, U, ItT, ItU, is_fail_safe>(begin, end, dest);
		}
	}	
}