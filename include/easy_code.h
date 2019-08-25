/*
author: YiliangWu
fun: �򻯴���

*/

#pragma once

#include <vector>
#include <list>
#include <set>

namespace su
{
	//�򻯵�������ѭ��д����ϰ���˼�д����BUG�١�
	//for(const MfAddr &addr : vec_mf_addr) ����Ӧ�ø���
	//����ɾ�� FOR_IT FOR_IT_CONST
#define FOR_IT(ctn)\
    for(auto it=(ctn).begin(); it!=(ctn).end(); ++it)

#define FOR_IT_CONST(ctn)\
    for(auto it=(ctn).cbegin(); it!=(ctn).cend(); ++it)

#define WHILE_NUM(num)\
    for(unsigned int i=0; i<num; ++i)

#define FOR_ARRAY(a)\
	for(unsigned int i=0; i<ArrayLen(a); ++i)

#define FOR_ARRAY_REVERSE(a)\
	for(unsigned int i=ArrayLen(a); i<ArrayLen(a); --i)

	//��ֹ�ำֵ������
#define DISNABLE_COPY_AND_ASSIGN(TypeName) \
    private:\
	TypeName(const TypeName&); \
	void operator=(const TypeName&)

//////////////////////////////////////////////////////////////////////////
//��ȡԭ�����鳤��
//template<typename Array>
//inline size_t ArrayLen(const Array &array)
//{
//	return sizeof(array)/sizeof(array[0]);
//}
//��Щ��Ҫ�����ĵط����ܺ������ͣ�����������
#define ConstArrayLen(array) (sizeof(array)/sizeof((array)[0]))

//��һ���÷�,���ֿ��ܸ��ã�����֤
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

//д�������������������Щ��
const unsigned int NUM_1W = 10000;
const unsigned int NUM_10W = 10 * NUM_1W;
const unsigned int NUM_100W = 100 * NUM_1W;
const unsigned int NUM_1000W = 1000 * NUM_1W;
const unsigned int NUM_1Y = 100 * NUM_100W;

} //end namespace YiLiang



//end file
