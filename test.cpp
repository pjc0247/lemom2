// lemon2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <chrono>
#include <type_traits>

using namespace std;

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
	printf("yield\n");
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


int _tmain(int argc, _TCHAR* argv[]){

	auto v = unit(1ms);

	await v;

	auto a = 1ms;

	unit(1ms);

	return 0;
}

