#pragma once

#include <assert.h>


/************************************************************************/
/* 
Example:
class concreterClass : public SingletonBase<concreterClass>
{
private:
concreterClass(){};

};
*/
/************************************************************************/

class Noncopyable
{
protected:
	Noncopyable() {}; 
	~Noncopyable() {};

private:
	Noncopyable(const Noncopyable&); 
	Noncopyable & operator= (const Noncopyable &);
};


template <typename T, bool INIT = true>
class SingletonBase : private Noncopyable
{
};

template <typename T>
class SingletonBase<T,true> : private Noncopyable
{
protected:
	SingletonBase() {}
	~SingletonBase() {}

public:
	//static T& instance()
 //   {
 //       return obj();
	//}
	static T& obj()
	{
		static T t;
		return t;
	}
};

