/*
author: YiliangWu
fun: 简化代码

*/

#pragma once

#include <vector>
#include <list>
#include <set>

namespace su
{

//////////////////////////////////////////////////////////////////////////
//获取原生数组长度
//template<typename Array>
//inline size_t ArrayLen(const Array &array)
//{
//	return sizeof(array)/sizeof(array[0]);
//}
//有些需要常量的地方不能函数类型，先用这个解决
#define ConstArrayLen(array) (sizeof(array)/sizeof((array)[0]))

//另一种用法,这种可能更好，待验证
template<typename T, std::size_t N> 
constexpr std::size_t ArrayLen(T(&)[N])
{ // constexpr
	return N; // and
}

//////////////////////////////////////////////////////////////////////////
//)easy use placement new
// construct object at _Ptr with default value
template<class T> 
inline	void constructInPlace(T  *_Ptr)
{	
	new ((void  *)_Ptr) T();
}

// construct object at _Ptr with value param
template<class _Ty,class _TParam> 
inline	void constructInPlace(_Ty  *_Ptr,_TParam param)
{	
	new ((void  *)_Ptr) _Ty(param);
}

template <class T>
inline void destructInPlace(T* p)
{
	p->~T();
}

//////////////////////////////////////////////////////////////////////////

//写大数字用这个，不容易些错
const unsigned int NUM_1W = 10000;
const unsigned int NUM_10W = 10 * NUM_1W;
const unsigned int NUM_100W = 100 * NUM_1W;
const unsigned int NUM_1000W = 1000 * NUM_1W;
const unsigned int NUM_1Y = 100 * NUM_100W;

} //end namespace YiLiang



//end file
