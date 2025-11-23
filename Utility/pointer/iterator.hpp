#pragma once
#include "pointer_base.hpp"
namespace dte_utils {
	template<typename T>
	struct iterator_base : pointer_base<T> {
		using type = pointer_base<T>::type;
		using pointer = pointer_base<T>::pointer;
		using pointer_base<T>::pointer_base;
		protected:
			void _inc(size_t add = 1) noexcept {
				if constexpr (std::is_void_v<type>) {
					this->_instance = static_cast<char*>(this->_instance) + add;
				}
				else {
					this->_instance += add;
				}
			}
			void _dec(size_t sub = 1) noexcept {
				if constexpr (std::is_void_v<type>) {
					this->_instance = static_cast<char*>(this->_instance) - sub;
				}
				else {
					this->_instance -= sub;
				}
			}
	};
	template<typename T>
	struct f_iterator : iterator_base<T> {
		using pointer = iterator_base<T>::pointer;
		using iterator_base<T>::iterator_base;
		public:
			f_iterator& operator++() noexcept {
				this->_inc();
				return *this;
			}
			f_iterator operator++(int) noexcept {
				f_iterator iter(*this);
				this->_inc();
				return iter;
			}
			f_iterator& operator--() noexcept {
				this->_dec();
				return *this;
			}
			f_iterator operator--(int) noexcept {
				f_iterator iter(*this);
				this->_dec();
				return iter;
			}
			f_iterator& operator+=(size_t add) noexcept {
				this->_inc(add);
				return *this;
			}
			f_iterator& operator-=(size_t sub) noexcept {
				this->_dec(sub);
				return *this;
			}
			f_iterator operator+(size_t add) const noexcept {
				f_iterator iter(*this);
				iter._inc(add);
				return iter;
			}
			f_iterator operator-(size_t sub) const noexcept {
				f_iterator iter(*this);
				iter._dec(sub);
				return iter;
			}
	};
	template<typename T>
	struct b_iterator : iterator_base<T> {
		using pointer = pointer_base<T>::pointer;
		using iterator_base<T>::iterator_base;
		public:
			b_iterator& operator++() noexcept {
				this->_dec();
				return *this;
			}
			b_iterator operator++(int) noexcept {
				b_iterator iter(*this);
				this->_dec();
				return iter;
			}
			b_iterator& operator--() noexcept {
				this->_inc();
				return *this;
			}
			b_iterator operator--(int) noexcept {
				b_iterator iter(*this);
				this->_inc();
				return iter;
			}
			b_iterator& operator+=(size_t sub) noexcept {
				this->_dec(sub);
				return *this;
			}
			b_iterator& operator-=(size_t add) noexcept {
				this->_inc(add);
				return *this;
			}
			b_iterator operator+(size_t sub) const noexcept {
				b_iterator iter(*this);
				iter._dec(sub);
				return iter;
			}
			b_iterator operator-(size_t add) const noexcept {
				b_iterator iter(*this);
				iter._inc(add);
				return iter;
			}
	};
}