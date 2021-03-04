/************************************************************************/
/* 
	ע�⣺��ֹ�����û������������ ���� concreterClass a; �û��Լ�����

��ֹ�������Example:
class concreterClass : public Singleton<concreterClass>
{
private:
concreterClass(){};

};
*/
/************************************************************************/
#pragma once

#include <assert.h>




class Noncopyable
{
protected:
	Noncopyable() {}; 
	~Noncopyable() {};

private:
	Noncopyable(const Noncopyable&)=delete; 
	Noncopyable & operator= (const Noncopyable &) = delete;
	Noncopyable(Noncopyable&&) = delete; //�ƶ����캯��
};


template <typename T>
class Singleton : private Noncopyable
{
protected:
	Singleton() {}
	virtual ~Singleton() {}

public:
	static T& Ins() 
	{
		if (!m_obj)
			m_obj = new T;

		return *m_obj;
	}

	static void Free()
	{
		if (m_obj)
			delete m_obj;

		m_obj = nullptr;
	}

private:
	static T* m_obj;

};

template<class T>
T* Singleton<T>::m_obj = nullptr;  //��ʼ��


