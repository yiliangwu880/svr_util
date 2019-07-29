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
class Singleton : private Noncopyable
{
};

template <typename T>
class Singleton<T,true> : private Noncopyable
{
protected:
	Singleton() {}
	~Singleton() {}

public:
	//static T& instance()
 //   {
 //       return Obj();
	//}
	static T& Obj()
	{
		static T t;
		return t;
	}
};

