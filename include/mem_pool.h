/*内存池
特点：用户接口真的简化！
	 不支持数组分配

目前初始化时零碎分配对象内存的，以后可以考虑优化未一整块分配。
未完成：
{
	宏定义可选：每个对象内存前面留uint32内存用来存放控制信息。 检查重复 delete 内存，高概率检查delete野地址(uint32存放特定控制信息，随机数字吧)。
}
例：  

class A: public MemPoolBaseObj<A, 1000>
{
};

void main()
{
	A *p = new A;  //这里就使用su::MemPool<A, 1000>::Instance()池分配对象了
	delete p;
}


*/

#pragma once  
#include <vector>  
#include <algorithm>  
#include <cassert>  

#define CHECK_FREE_ERROR //定义了，会检查释放对象参数是否正确. 效率会更慢

namespace su
{
	//@Obj 具体类
	//@init_obj_num 需要对象数量，用它来初始化内存块
	template<class Obj, size_t init_obj_num>
	class MemPoolBaseObj
	{
	public:
		void* operator new(size_t size);
		void operator delete(void* p);

		static size_t GetAllocateNum();
	};

	//@Obj 为要分配对象的类
	//@init_obj_num 需要对象数量，用它来初始化内存块
	template<class Obj, size_t init_obj_num>
	class MemPool
	{
		friend class MemPoolBaseObj<Obj, init_obj_num>;
	private:
		std::vector<Obj*> m_vec_all_obj;      //缓存所有对象 ,包括分配和未分配 
		std::vector<Obj*> m_vec_free_obj;     //未分配的对象 

	public:
		static  MemPool<Obj, init_obj_num>& Instance();           //返回单件实例  
		size_t GetAllocateNum();	//获取已分配对象数

	private:  
		MemPool()
		{
			SetBufferSize(init_obj_num);
		}
		~MemPool(void)//关闭进程时清除所有的缓存对象  
		{
			Clear();
		}
		MemPool(const MemPool& other);
		bool SetBufferSize(unsigned int obj_num);  //设置缓冲池大小, size为对象数量
		Obj* Allocate();                           //分配  
		void Free(Obj* pT);                        //释放  
		void Clear();                              //清除所有的缓存对象  
	};

	template<class Obj, size_t init_obj_num>
	size_t su::MemPool<Obj, init_obj_num>::GetAllocateNum()
	{
		if (m_vec_free_obj.size() > m_vec_all_obj.size())
		{
			printf("logic error");
			return -1;
		}
		return m_vec_all_obj.size() - m_vec_free_obj.size();
	}

	//成员函数实现  
	template<class Obj, size_t init_obj_num>
	inline MemPool<Obj, init_obj_num>& MemPool<Obj, init_obj_num>::Instance()
	{
		static MemPool<Obj, init_obj_num> obj;
		return obj;
	}

	//设置缓存池大小  
	template<class Obj, size_t init_obj_num>
	inline bool MemPool<Obj, init_obj_num>::SetBufferSize(unsigned int obj_num)
	{
		if (!m_vec_all_obj.empty())
		{
			printf("can't repeated call SetBufferSize");
			return false;
		}
		m_vec_all_obj.resize(obj_num);

		for (unsigned int i = 0; i < m_vec_all_obj.size(); ++i)
		{
			m_vec_all_obj[i] = (Obj *)::malloc(sizeof(Obj));
		}
		m_vec_free_obj.resize(obj_num);
		std::copy(m_vec_all_obj.begin(), m_vec_all_obj.end(), m_vec_free_obj.begin());
		return true;
	}



	//分配内存  
	template<class Obj, size_t init_obj_num>
	inline Obj* MemPool<Obj, init_obj_num>::Allocate()
	{
		//内存已经分配完了， 动态增长  
		if (m_vec_free_obj.empty()) {
			printf("error, mem_pool [%s] is empty. total allocate num=%d\n", typeid(int).name(), (int)m_vec_all_obj.size());
			for (unsigned int i = 0; i < 1000; ++i)
			{
				Obj *p = (Obj *)::malloc(sizeof(Obj));
				m_vec_all_obj.push_back(p);
				m_vec_free_obj.push_back(p);
			}
		}

		Obj* pT = m_vec_free_obj.back();
		m_vec_free_obj.pop_back();
		return pT;

	}

	//释放对象  
	template<class Obj, size_t init_obj_num>
	inline void MemPool<Obj, init_obj_num>::Free(Obj* pT)
	{
#ifdef CHECK_FREE_ERROR 
		//数量多了，效率会很慢，以后稳定了要去掉这个代码
		//确保删除的对象在缓存中  
		assert(std::find(m_vec_all_obj.begin(), m_vec_all_obj.end(), pT) != m_vec_all_obj.end() && "object to free is not exist!!");
		//确保只删除一次  
		assert(std::find(m_vec_free_obj.begin(), m_vec_free_obj.end(), pT) == m_vec_free_obj.end() && "object has deleted already!!");
#endif
		//分配和回收都用 back操作，相对于一个back,一个front操作更好。
		//因为都用 back操作让新对象高概率使用最近释放对象内存。如果用户写出野指针代码，问题就会更高概率出现。是好事。
		m_vec_free_obj.push_back(pT); 
	}

	//清除缓存池内容  
	template<class Obj, size_t init_obj_num>
	inline void MemPool<Obj, init_obj_num>::Clear()
	{
		for (Obj* p : m_vec_all_obj)
		{
			::free(p);
		}
		m_vec_all_obj.clear();
		m_vec_free_obj.clear();
	}



	template<class Obj, size_t init_obj_num>
	void* MemPoolBaseObj<Obj, init_obj_num>::operator new(size_t size)
	{
		return MemPool<Obj, init_obj_num>::Instance().Allocate();
	}

	template<class Obj, size_t init_obj_num>
	void MemPoolBaseObj<Obj, init_obj_num>::operator delete(void* p)
	{
		MemPool<Obj, init_obj_num>::Instance().Free(reinterpret_cast<Obj*>(p));
	}

	template<class Obj, size_t init_obj_num>
	size_t su::MemPoolBaseObj<Obj, init_obj_num>::GetAllocateNum()
	{
		return MemPool<Obj, init_obj_num>::Instance().GetAllocateNum();
	}

}

