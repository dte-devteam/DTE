# Указатели

Указатель - значение которое хранит адрес памяти другого переменной, а не ее значение.

## raw_pointer

Шаблонная структура `raw_pointer` предоставляет минимальный набор обязательного функционала:
 - инициализация
 - хранение
 - получение
 - запись
 - сравнение

Особенности: 
 - поддерживает указатели[^ptr_to_member] на члены классов/структур

Создаваемые типы:
 - `raw_type` - тип с отброшенными указателями
 - `pointer` - тип в виде которого хранится указатель
 - `type` - тип на который указывает указатель

```cpp
#include "pointer/reference/weak_ref.hpp"

using namespace dte_utils;

struct int_handler {
    int i = 0;
}

void ordinary_pointer_example() {
    //create container
    raw_pointer<int*> ptr_to_ptr = new int*(new int(100));
    //cast contaier to raw_pointer<int*>::pointer
    int** ii = static_cast<int**>(ptr_to_ptr);
    //or use operator pointer
    ii = ptr_to_ptr.operator raw_pointer<int*>::pointer;
    //use container as bool
    if (ptr_to_ptr) {
        //do smth
    }
    //get stored data
    raw_pointer<int> ptr = ptr_to_ptr.get_value();  //ptr_to_ptr.get_value()->raw_pointer<int>
    int* i = static_cast<int*>(ptr);
    int value = ptr.get_value();   //ptr.get_value()->int&, value equals 100
    //be good guys and free memory
    delete i;
    delete ii;
}

void member_pointer_example() {
    //create container
    raw_pointer<int(int_handler::*)> int_ptr = &int_handler::i;
    //get stored data
    int_ptr.get_value();    //int_ptr.get_value()->int(int_handler::*)&
}
```
## ponter_base

Шаблонная структура `ponter_base` наследуется от `raw_pointer` используя конструкторы родителя. Предоставляет более удобные методы получения и записи:
 - для обычных указателей
   - оператор разыменовывания (если указывает не на функцию и не на член)
   - оператор косвенного обращения (если указывает не на функцию и не на член)
   - оператор вызова функции (если указатель на функцию)
   - оператор вызова метода функтора (если указатель на функтор)
 - для указателей на члены классов/структур
   - получение ссылки на член/вызов функции-члена
   - получение константной ссылки на член/вызов константной функции-члена[^because_of_const_owner]

```cpp
#include "ponter/pointer_base.hpp"

using namespace dte_utils;

struct int_handler {
    int i = 0;
    int operator()(int add) const noexcept {
        return i + add;
    }
}

void ordinary_pointer_example() {
    //create container
    pointer_base<int> int_ptr = new int(100);
    //get & set
    int i = *int_ptr;
    *int_ptr = i;
    //create container
    pointer_base<int_handler> hand_ptr = new int_handler;
    //get & set
    hand_ptr->i = 100;
    //execute
    int x = hand_ptr(10); //will call operator()(int), x = 110
    //operator pointer() is longer than operator->()
    hand_ptr.operator->();
    hand_ptr.operator decltype(hand_ptr)::pointer();
    //be good guys and free memory
    delete int_ptr.operator->();
    delete hand_ptr.operator->();
}
```
### complex_pointer

Шаблонная структура `complex_pointer` наследуется от `pointer_base`. Предоставляет новый функционал для указателей на члены класса/структуры. Использует конструкторы родителя, если использован указатель не на член класса/структуры.
При указателе на член класса/структуры:
 - позволяет хранить экземпляр класса/структуры
 - позволяет вызывать функцию (если указатель функция-член)
 - позволяет обращаться к члену класса/структуры (если член не функция-член)
```cpp
#include "ponter/complex_pointer.hpp"

using namespace dte_utils;

struct int_handler {
    int i = 0;
    int add(int a) {
        return a + i;
    }
}

void ordinary_pointer_example() {
    //create struct
    int_handler hand{100};
    //create container (second argument - owner)
    complex_pointer<int(int_handler::*), false> handler_ptr{&int_handler::i, &hand}; //we also can change owner (set_owner)
    //access field
    handler_ptr() = 99; //now hand.i = 99
    complex_pointer<int(int_handler::*)(int), false> handler_func{&int_handler::add, &hand};
    int value = handler_func(1);    //value equals 100
    //get pointer to owner
    pointer_base<int_handler> own = handler_func.get_owner();
}
```

[^ptr_to_member]: указатели на члены классов/структур не требуют дополнительного выделения памяти и используются напрмямую, например `raw_pointer<field(struct::*)>` иметь `pointer` в виде `field(struct::*)`
[^because_of_const_owner]: в случае если класс/структура предоставлена только в константном значении