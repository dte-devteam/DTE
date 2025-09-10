#pragma once
#include "memory/dynamic_string.hpp"
#include "semi_pointer.hpp"
#include "types.hpp"
namespace dte_token {
	struct data_stack;
	struct c_function {
		struct metadata {
			const char* name;
			//max jump index this function can output
			size_t max_jump;
		};
		using func = size_t(data_stack&, const semi_pointer::data&);
		protected:
			metadata _meta;
			func* _body;
			destructor* _args_destructor;
		public:
			constexpr c_function(func* body, const metadata& meta, destructor* args_destructor = nullptr) : _body(body), _meta(meta), _args_destructor(args_destructor) {}
			constexpr c_function(func* body, metadata&& meta, destructor* args_destructor = nullptr) : _body(body), _meta(std::move(meta)), _args_destructor(args_destructor) {}
			constexpr c_function(const c_function& other) : _meta(other.get_meta()), _body(other.get_body()), _args_destructor(other.get_args_destructor()) {}
			constexpr c_function(c_function&& other) noexcept : _body(other.get_body()), _meta(std::move(other._meta)), _args_destructor(other.get_args_destructor()) {}
			constexpr const metadata& get_meta() const noexcept {
				return _meta;
			}
			constexpr func* get_body() const noexcept {
				return _body;
			}
			constexpr destructor* get_args_destructor() const noexcept {
				return _args_destructor;
			}
			c_function& operator=(const c_function&) = delete;
			c_function& operator=(c_function&&) = delete;
			size_t operator()(data_stack& stack, const semi_pointer::data& spd) const;
	};
}