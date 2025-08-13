#pragma once
#include "memory/dynamic_string.hpp"
#include "semi_pointer.hpp"
#include "types.hpp"
namespace dte_token {
	struct data_stack;
	struct c_function {
		struct metadata {
			dte_utils::dynamic_cstring name;
			//max jump index this function can output
			size_t max_jump;
			//size of data type pushed on stack
			//if it is <=sizeof(void*) uses casted void* as data
			size_t data_size_push;
		};
		using func = size_t(data_stack&, const semi_pointer::data&);
		protected:
			metadata _meta;
			func* _body;
			destructor* _args_destructor;
		public:
			c_function(func* body, const metadata& meta, destructor* args_destructor = nullptr);
			c_function(func* body, metadata&& meta, destructor* args_destructor = nullptr);
			c_function(const c_function& other) : _meta(other.get_meta()), _body(other.get_body()), _args_destructor(other.get_args_destructor()) {};
			c_function(c_function&& other) noexcept;
			const metadata& get_meta() const noexcept;
			func* get_body() const noexcept;
			destructor* get_args_destructor() const noexcept;
			c_function& operator=(const c_function&) = delete;
			c_function& operator=(c_function&&) = delete;
			size_t operator()(data_stack& stack, const semi_pointer::data& spd) const;
	};
}