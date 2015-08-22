#include <stdlib.h>
#pragma once

//
// Tools
//
template <typename Type> 
void __objcpy(Type *dest, const Type *from, unsigned int num){
	for (unsigned int i = 0; i < num; i++){
		dest[i] = from[i];
	}
}

//
// Simple Generic Array 
//
template<typename Type>
struct __is_pointer { 
	typedef Type clean_type;
	static const bool value = false; 
};
template<typename Type>
struct __is_pointer<Type*> { 
	typedef Type clean_type;
	static const bool value = true; 
};

template <bool Condition, typename True_t, typename False_t>
struct __conditional { typedef True_t type; };
template <typename True_t, typename False_t>
struct __conditional<false, True_t, False_t> { typedef False_t type; };

template <typename Type, int Size>
struct __data_on_stack {
	Type data[Size]; 
};
template <typename Type>
struct __data_on_heap {
	Type *data;
	__data_on_heap(): data(NULL) {}
	~__data_on_heap(){ delete[] this->data; }
};

enum { HEAP = 0};

template <typename Type, int Allocator, int Size = Allocator>
struct __array 
	: public __conditional<Allocator != 0, 
			__data_on_stack<Type, Size>, 
			__data_on_heap<Type> 
		>::type
{
protected:
	typedef unsigned int size_t;
	size_t pos;
public:
	__array(): pos(-1) {}
	size_t size() const {
		return pos + 1;
	}
	size_t capacity() const {
		return Size;
	}
	bool empty() const {
		return pos == -1;
	}
	const Type &operator [] (size_t idx){
		return this->data[idx];
	}
	void push_back(const Type &elem){
		++pos;
		if (pos < Size){
			this->data[pos] = elem;
		}
	}
	Type pop_back(){
		if (pos == -1)
			return this->data[0]; // possible segfault
		else {
			Type copy = this->data[pos];
			delete_if_ptr(this->data[pos]);
			--pos;
			return copy;
		}
	}
	const Type &back() const {
		return this->data[pos];
	}
	~__array(){
		for (unsigned int i = 0; i < this->size(); i++){
			delete_if_ptr(this->data[i]);
		}
	}
	static void delete_if_ptr(
		const typename __is_pointer<Type>::clean_type&
	){}
	static void delete_if_ptr(
		const typename __is_pointer<Type>::clean_type* value)
	{ delete[] value; }
};

// Stack version implicitely
template <class Type, int Allocator> struct Array 
	: public __array<Type, Allocator> {};
	
// Heap version explicitely
template <class Type> struct Array<Type, HEAP> : public __array<Type, HEAP> {
private:
	size_t dyncapacity;
public:
	Array(size_t dyn_capacity = 0): __array<Type, HEAP>(), dyncapacity(dyn_capacity) {
		if (dyncapacity != 0){
			this->data = 
				static_cast<Type*>(::operator new (dyncapacity*sizeof(Type)));
		}
	}
	size_t capacity() const {
		return dyncapacity;
	}
	Type &operator [](size_t idx){
		return this->data[idx];
	}
	void push_back(const Type &elem){
		++(this->pos);
		if (this->pos < dyncapacity){
			this->data[this->pos] = elem;
		}
		else {
			size_t alloc_size = dyncapacity*sizeof(Type);
			if (dyncapacity == 0){
				dyncapacity = 10; 
				this->data = static_cast<Type*>(::operator new(alloc_size));
			}
			else { 
				dyncapacity *= 2; 
				Type *temp = static_cast<Type*>(::operator new(alloc_size));
				__objcpy<Type>(temp, this->data, (this->pos) + 1);
				delete[] this->data;
				this->data = temp;
			}
			this->data[this->pos] = elem;
		}
	}
};
