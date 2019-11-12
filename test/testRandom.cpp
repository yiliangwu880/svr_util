

#include "su_rand.h"
#include "unit_test.h"
#include "su_mgr.h"

using namespace std;
using namespace su;
namespace
{


void testRandomWeight()
{
	vector<uint32> vec;
	vec.resize(3);
	{
		vec[0]=0;
		vec[1]=0;
		vec[2]=1;
		RandomWeight obj(vec);
		UNIT_ASSERT(2==obj.rand());
		UNIT_ASSERT(2==obj.rand());
		UNIT_ASSERT(2==obj.rand());
		UNIT_ASSERT(2==obj.rand());
	}
	{
		vec[0]=1;
		vec[1]=1;
		vec[2]=1;
		RandomWeight obj(vec);
		UNIT_ASSERT(obj.rand()<3);
		UNIT_ASSERT(obj.rand()<3);
		UNIT_ASSERT(obj.rand()<3);
		UNIT_ASSERT(obj.rand()<3);
		UNIT_ASSERT(obj.rand()<3);
		UNIT_ASSERT(obj.rand()<3);
	}
	{
		vec[0]=1;
		vec[1]=10;
		vec[2]=1;
		RandomWeight obj(vec);
		UNIT_ASSERT(obj.rand() < 3);
		UNIT_ASSERT(obj.rand() < 3);
		UNIT_ASSERT(obj.rand() < 3);
		UNIT_ASSERT(obj.rand() < 3);
		UNIT_ASSERT(obj.rand() < 3);
		UNIT_ASSERT(obj.rand() < 3);
	}

}
void testPrimeRandom()
{
	PrimeRandom r(1000);
	std::set<uint16> s;
	for (uint32 i=0; i<1000; i++)
	{
		s.insert(r.GetRandom());
	}
	uint16 count = 0;
	for (std::set<uint16>::const_iterator it=s.begin(); it!=s.end(); ++it, ++count)
	{
		UNIT_ASSERT(*it==count);
	}
	UNIT_ASSERT(1000==count);

	std::set<uint16> s2;
	for (uint32 i = 0; i < 1000; i++)
	{
		s2.insert(r.GetRandom());
	}
	UNIT_ASSERT(1000 == s2.size());
}
void  testRandomBetween()
{
	bool isOne = false;
	bool isZero = false;
	for (uint32 i=0; i<1000; ++i)
	{
		switch(Random::RandUint32(1,0))
		{
		default:
			UNIT_ASSERT(false);
			break;
		case 0:
			isZero = true;
			break;
		case 1:
			isOne = true;
			break;
		}
	}
	UNIT_ASSERT(isOne);
	UNIT_ASSERT(isZero);

	for (uint32 i=0; i<1000; ++i)
	{
		uint32 r = Random::RandUint32(0,100);
		UNIT_ASSERT(r>=0 && r<=100);
	}
}

void test()
{
	int r = 0;
	UNIT_ASSERT(Random::RandInt(0, 0) == 0);
	UNIT_ASSERT(Random::RandInt(-1, -1) == -1);
	for (uint32 i = 0; i < 1000*100; ++i)
	{
		r = Random::RandInt(0, 1);
		UNIT_ASSERT((r == 0) || (1 == r));
		double d = Random::RandDecimal();
		UNIT_ASSERT(d < 1);
	}
}
}//end namespace

UNITTEST(testRandom)
{
	SuMgr::Obj().Init();
	testPrimeRandom();
	testRandomWeight();
	testRandomBetween();
	test();
	UNIT_INFO("done");
}