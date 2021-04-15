/*//二叉堆 算法，
MinHeap 引用节点
节点释放时，自动从堆删除。

备注：
	测试随机加入，出列发现和 stl::priority_queue 性能部分上下，有时queue高，有时minhap高。
	可是priority_queue没有删除任意节点功能呀，定时器场景用不了。 

*/

#pragma once
#include <cstdio>
#include "vector"

namespace su
{

template<class >
class MinHeap;

//Node 实现要求有函数 bool IsLess(const Data &other) ， true表示 this < other
template<class Node>
class MinHeapNodeBase
{
	friend class MinHeap<Node>;

	int index = -1;
	MinHeap<Node> *pMinHeap = nullptr;

private: //不能复制
	MinHeapNodeBase(const MinHeapNodeBase&) = delete;
	MinHeapNodeBase & operator= (const MinHeapNodeBase &) = delete;

public:
	MinHeapNodeBase() {};
	~MinHeapNodeBase();
	MinHeapNodeBase(MinHeapNodeBase&& tmp);//移动构造函数, 没加入堆，允许移动构造。方便加入stl容器管理
};

template<class Node>
class MinHeap
{
	friend class MinHeapNodeBase<Node>;

	std::vector<Node*>  m_vec;
public:
	MinHeap();
	~MinHeap();
	bool Push(Node &node);
	//树根节点出列
	Node *Pop();
	//获取树根，（最小或者最大值）
	Node *Front();
	bool Erase(Node* node);

private:
	bool Siftdown(int x, int n);
	void Siftup(int j);
	//为了支持 MinHeapNode移动构造函数
	void ReplaceNode(Node *oldNode, Node *newNode);
};

template<class Node>
void MinHeap<Node>::ReplaceNode(Node *oldNode, Node *newNode)
{
	m_vec[oldNode->index] = newNode;
}

template<class Node>
MinHeap<Node>::MinHeap()
{
	// reserve a little space
	m_vec.reserve(64);
}

template<class Node>
MinHeap<Node>::~MinHeap()
{
	for (auto &v: m_vec)
	{
		v->pMinHeap = nullptr;
	}
	m_vec.clear();
}

#define HEAP_ITEM_LESS(i, j) (m_vec[(i)]->IsLess(*(m_vec[(j)])))

template<typename Node>
bool MinHeap<Node>::Push(Node &node)
{
	if (node.pMinHeap != nullptr)
	{
		return false;
	}
	node.pMinHeap = this;
	node.index = (int)m_vec.size();
	m_vec.push_back(&node);
	Siftup((int)m_vec.size() - 1);
	return true;
}

template<typename Node>
Node *MinHeap<Node>::Front()
{
	if (m_vec.empty())
	{
		return nullptr;
	}
	return m_vec.front();
}

template<typename Node>
Node *MinHeap<Node>::Pop()
{
	if (m_vec.empty())
	{
		return nullptr;
	}
	Node* node = m_vec.front();
	Erase(node);
	return node;
}

template<typename Node>
bool MinHeap<Node>::Erase(Node* node)
{
	if (node->pMinHeap != this)
	{
		return false;
	}
	int n = (int)m_vec.size() - 1;
	int i = node->index;
	if (i != n)
	{
		std::swap(m_vec[i], m_vec[n]);
		m_vec[i]->index = i;
		if (!Siftdown(i, n))
		{
			Siftup(i);
		}
	}
	m_vec.pop_back();
	node->pMinHeap = nullptr;
	return true;
}

template<typename Node>
bool MinHeap<Node>::Siftdown(int x, int n)
{
	int i = x;
	for (;;)
	{
		int j1 = 2 * i + 1;
		if ((j1 >= n) || (j1 < 0)) // j1 < 0 after int overflow
		{
			break;
		}
		int j = j1; // left child
		int j2 = j1 + 1;
		if (j2 < n && !HEAP_ITEM_LESS(j1, j2))
		{
			j = j2; // right child
		}
		if (!HEAP_ITEM_LESS(j, i))
		{
			break;
		}
		std::swap(m_vec[i], m_vec[j]);
		m_vec[i]->index = i;
		m_vec[j]->index = j;
		i = j;
	}
	return i > x;
}

template<typename Node>
void MinHeap<Node>::Siftup(int j)
{
	for (;;)
	{
		int i = (j - 1) / 2; // parent node
		if (i == j || !HEAP_ITEM_LESS(j, i))
		{
			break;
		}
		std::swap(m_vec[i], m_vec[j]);
		m_vec[i]->index = i;
		m_vec[j]->index = j;
		j = i;
	}
}

template<class Node>
MinHeapNodeBase<Node>::MinHeapNodeBase(MinHeapNodeBase&& tmp)
{
	pMinHeap = tmp.pMinHeap;
	index = tmp.index;
	if (pMinHeap)
	{
		pMinHeap->ReplaceNode((Node *)&tmp, (Node *)this);
	}

	tmp.index = -1;
	tmp.pMinHeap = nullptr;
}

template<class Node>
MinHeapNodeBase<Node>::~MinHeapNodeBase()
{
	if (pMinHeap)
	{
		pMinHeap->Erase((Node*)this);
	}
}

#undef HEAP_ITEM_LESS


}