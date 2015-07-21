// lemon2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <chrono>
#include <type_traits>
#include <functional>

#include <Windows.h>

using namespace std;


class foo {
public:
	foo() {}
	foo(int n) : n(n) {}
	foo(const foo &f){
		printf("copy\n");

		n = f.n;
	}
	foo(foo &&f) {
		printf("move\n");

		n = f.n;
	}

	foo *operator =(const foo &f){
		printf("copy =\n");

		n = f.n;

		return nullptr;
	}
	foo *operator =(foo &&f) {
		printf("move =\n");

		n = f.n;

		return nullptr;
	}

	int n;
};

#define await ~

template <typename T>
class iawaitable{
public:
	virtual T operator ~() =0;
};

/* operator ~에 const 안붙이기 위해 const ref 미사용 */
template <typename T>
std::enable_if_t<
	!std::is_void<T>::value,
	T>
_await(const iawaitable<T> &o) {
	return ~o;
}
template <typename T>
std::enable_if_t<
	!std::is_void<T>::value,
	T>
_await(iawaitable<T> &&o) {
	return ~o;
}
template <typename T>
std::enable_if_t<
	std::is_void<T>::value,
	void>
_await(iawaitable<T> &o){
	~o;
}
template <typename T>
std::enable_if_t<
	std::is_void<T>::value,
	void>
_await(iawaitable<T> &&o) {
	~o;
}

template <typename T>
void yield(T &&o){
	void *ptr = GetFiberData();
	auto c = (coroutine<std::remove_reference_t<T>>*)ptr;

	c->yield(std::forward<remove_reference_t<T>>(o));
}

class unit : public iawaitable<void>{
public:
	unit(const std::chrono::milliseconds &t) :
		ms(t){
	}

	virtual void operator ~(){
		
	};

private:
	std::chrono::milliseconds ms;
};

template <typename T>
class task : iawaitable<T>{
public:

	virtual T operator~(){
		return T();
	}
};

class tasks{
public:
};

class icoroutine{
	virtual void yield() =0;
};

template <typename T>
class coroutine{
public:
	class iterator{
	public:
		iterator(coroutine<T> &parent) :
			parent(parent){
		}

		bool operator != (const iterator &other) const{
			return !parent.eof;
		}
		T operator* (){
			return std::move(buffer);
		}
		iterator& operator++ ()
		{
			parent.next(&buffer);

			return *this;
		}

	private:
		T buffer;
		coroutine<T> &parent;
	};

public:
	coroutine() :
		this_fiber_ptr(nullptr), prev_fiber_ptr(nullptr){
	}
	coroutine(std::function<void()> &f) :
		eof(false),
		this_fiber_ptr(nullptr), prev_fiber_ptr(nullptr),
		func(f){

		init_fiber();
	}
	coroutine(std::function<void()> &&f) :
		eof(false),
		this_fiber_ptr(nullptr), prev_fiber_ptr(nullptr),
		func(std::move(f)) {

		init_fiber();
	}

	bool next(T *dst){
		prev_fiber_ptr = GetCurrentFiber();
		SwitchToFiber(this_fiber_ptr);

		if(eof){
			return false;
		}
		else{
			*dst = std::move(yield_storage);
			return true;
		}
	}

	void yield(){
		SwitchToFiber(prev_fiber_ptr);
	}
	void yield(T &&o){
		yield_storage = std::forward<T>(o);

		yield();
	}

	iterator begin(){
		return std::move(++iterator(*this));
	}
	iterator end(){
		return iterator(*this);
	}

private:
	void init_fiber(){
		this_fiber_ptr = CreateFiber(0, coroutine<T>::co_func, this);
	}

	static void __stdcall co_func(void *arg){
		coroutine<T> *_this = (coroutine<T>*)arg;

		_this->func();
		_this->eof = true;
		_this->yield();
	}

private:
	T yield_storage;
	std::function<void()> func;
	void *this_fiber_ptr;
	void *prev_fiber_ptr;

	bool eof;
};

class coroutines{
public:
	template <typename T>
	static coroutine<T> create(std::function<void()> &f){
		return coroutine<T>(f);
	}
	template <typename T>
	static coroutine<T> create(std::function<void()> &&f) {
		return coroutine<T>(std::forward<std::function<void()>>(f));
	}
};

int _tmain(int argc, _TCHAR* argv[]){
	ConvertThreadToFiber(nullptr);

	auto a = coroutines::create<foo>([](){
		for(int i=0;i<5;i++){
			yield(i);
		}
	});

	for(foo v : a){
		printf("%d\n", v.n);
	}

	return 0;
}

