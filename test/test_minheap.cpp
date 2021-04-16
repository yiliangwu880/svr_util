#include "time/minheap.h"
#include "unit_test.h"
#include <functional>
#include <limits>
#include <queue>

using namespace std;
using namespace su;

using namespace placeholders;

class Data : public MinHeapNodeBase<Data>
{
public:
	int v = 0;
	int id = 0;
	bool IsLess(const Data &other) { 
		return v < other.v;
	}
};



UNITTEST(MinHeapDel)
{
	MinHeap< Data> hp;
	{
		vector<Data> vec;
		vec.push_back(Data());
		hp.Push(vec.front());
		UNIT_ASSERT(hp.Pop());
	}

	UNIT_ASSERT(!hp.Pop());

}

UNITTEST(minheap_base_view)
{
	MinHeap< Data> hp;
	map<int, Data> m;
	for (int i = 0; i < 5 ; i++)
	{
		auto ret = m.insert(make_pair(i, Data()));
		int r = ::rand()%100;
		Data &data = ret.first->second;
		data.id = i;
		data.v = r;
		//UNIT_INFO("%d %d", i, r);
		hp.Push(data);
	}

	Data tmp;
	tmp.v = 99999;
	hp.Push(tmp);
	Data moveTmp(std::move(tmp));

	UNIT_ASSERT(!hp.Push(moveTmp));

	bool isFindmoveTmp = false;
	for (;;)
	{
		Data *p = hp.Front();
		if (!p)
		{
			break;
		}
		if (&moveTmp == p)
		{
			isFindmoveTmp = true;
		}
		UNIT_ASSERT(&tmp != p);
		if (::rand() % 2 == 0)
		{
			UNIT_ASSERT(hp.Erase(*p));
		}
		else
		{
			hp.Pop();
		}
		//UNIT_INFO("pop %d %d", p->id, p->v);
	}
	UNIT_ASSERT(isFindmoveTmp);
}
#if 0

UNITTEST(minheap_rand)
{
	srand((unsigned int)time(0));
	MinHeap< Data> hp;
	map<int, Data> m;
	time_t start = time(0);
	for (int i = 0; i < 10000*300; i++)
	{
		auto ret = m.insert(make_pair(i, Data()));
		int r = ::rand()%1000;
		Data &data = ret.first->second;
		data.id = i;
		data.v = r;
		hp.Push(data);
	}

	int lastNum = numeric_limits<int>::min();
	for (;;)
	{
		Data *p = hp.Front();
		if (!p)
		{
			break;
		}
		UNIT_ASSERT(p->v >= lastNum);
		lastNum = p->v;
		if (::rand() % 2 == 0)
		{
			UNIT_ASSERT(hp.Erase(*p));
		}
		else
		{
			hp.Pop();
		}
	}

	UNIT_INFO("%d", lastNum);
	time_t end = time(0);
	UNIT_INFO("cost %ld sec", end -start);//cost 4 sec
}

struct F1 
{
	bool operator()(const Data *a, const Data *b)const { return a->v > b->v; }
};
class mycomparison
{
public:
	mycomparison()
	{
	}
	bool operator() (const Data& a, const Data&b) const
	{
		return a.v < b.v;
	}
};
UNITTEST(priority_queue_rand)
{
	srand((unsigned int)time(0));
	F1 f;
	std::priority_queue<Data*, std::vector<Data*>, F1> hp(f);
	map<int, Data> m;
	time_t start = time(0);
	for (int i = 0; i < 10000 * 300; i++)
	{
		auto ret = m.insert(make_pair(i, Data()));
		int r = ::rand() % 1000;
		Data &data = ret.first->second;
		data.id = i;
		data.v = r;
		hp.push(&data);

	}

	int lastNum = numeric_limits<int>::min();
	for (;;)
	{
		if (hp.empty())
		{
			break;
		}
		Data *p = hp.top();
		UNIT_ASSERT(p->v >= lastNum);
		lastNum = p->v;
		if (::rand() % 2 == 0)
		{
			
			hp.pop();
		}
		else
		{
			hp.pop();
		}
	}

	UNIT_INFO("%d", lastNum);
	time_t end = time(0);
	UNIT_INFO("priority_queue cost %ld sec", end - start);
}


#endif