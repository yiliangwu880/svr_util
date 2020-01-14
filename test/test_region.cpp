
#include "less_use/region.h"
#include "unit_test.h"


using namespace std;

namespace
{
	void test1()
	{

		{
			MapRegion m;
			MapRegion::Region region;
			UNIT_ASSERT(m.AddRegion(5, 10));

			UNIT_ASSERT(m.Find(10, region));
			UNIT_ASSERT(5 == region.first);
			UNIT_ASSERT(10 == region.second);

			UNIT_ASSERT(m.Find(5, region));
			UNIT_ASSERT(5 == region.first);
			UNIT_ASSERT(10 == region.second);
		}
		MapRegion m;
		MapRegion::Region region;

		UNIT_ASSERT( !m.Find(1, region) );
		UNIT_ASSERT( !m.Find(0, region) );
		UNIT_ASSERT( !m.Find(12, region) );
		UNIT_ASSERT(m.AddRegion(0,0));
		UNIT_ASSERT(!m.AddRegion(0,13));
		UNIT_ASSERT(!m.AddRegion(11,10));


		UNIT_ASSERT(m.AddRegion(10,10));
		UNIT_ASSERT( !m.Find(1, region) );
		UNIT_ASSERT( !m.Find(11, region) );
		UNIT_ASSERT( m.Find(0, region) );
		UNIT_ASSERT(m.Find(10, region));
		UNIT_ASSERT( 10 == region.first );
		UNIT_ASSERT(!m.Erase(11));
		UNIT_ASSERT(m.Erase(10));


		UNIT_ASSERT( !m.Find(1, region) );
		UNIT_ASSERT( m.Find(0, region) );
		UNIT_ASSERT( !m.Find(12, region) );
		UNIT_ASSERT(!m.AddRegion(0,0));
		UNIT_ASSERT(!m.AddRegion(0,13));
		UNIT_ASSERT(!m.AddRegion(11,10));

		m.Clear();
		//clear at current state

		UNIT_ASSERT(m.AddRegion(10,10));
		UNIT_ASSERT(!m.AddRegion(10,11));
		UNIT_ASSERT(!m.AddRegion(10,12));
		UNIT_ASSERT(!m.AddRegion(9,10));
		UNIT_ASSERT(!m.AddRegion(9,11));
		UNIT_ASSERT(!m.AddRegion(0,10));
		UNIT_ASSERT(!m.AddRegion(0,11));
		UNIT_ASSERT( m.Find(10, region) );UNIT_ASSERT( 10 == region.first );

		//state = [10,10]
		UNIT_ASSERT(m.AddRegion(11,11));
		UNIT_ASSERT(!m.AddRegion(11,12));
		UNIT_ASSERT(!m.AddRegion(11,13));
		UNIT_ASSERT(!m.AddRegion(9,11));
		UNIT_ASSERT(!m.AddRegion(9,12));
		UNIT_ASSERT(!m.AddRegion(0,11));
		UNIT_ASSERT(!m.AddRegion(0,12));
		UNIT_ASSERT(!m.AddRegion(10,11));

		UNIT_ASSERT( m.Find(10, region) );UNIT_ASSERT( 10 == region.first );

		//state = [10,10], [11,11]
		UNIT_ASSERT(m.AddRegion(5,7));

		//state = [5,7], [10,10], [11,11]
		UNIT_ASSERT(!m.AddRegion(5,5));
		UNIT_ASSERT(!m.AddRegion(5,6));
		UNIT_ASSERT(!m.AddRegion(5,7));
		UNIT_ASSERT(!m.AddRegion(5,8));
		UNIT_ASSERT(!m.AddRegion(5,10));
		UNIT_ASSERT(!m.AddRegion(0,10));
		UNIT_ASSERT(!m.AddRegion(4,7));
		UNIT_ASSERT(!m.AddRegion(7,8));
		UNIT_ASSERT(!m.AddRegion(0,5));
		UNIT_ASSERT(!m.AddRegion(4,5));

		UNIT_ASSERT( m.Find(5, region) );UNIT_ASSERT( 5 == region.first );
		UNIT_ASSERT( m.Find(6, region) );UNIT_ASSERT( 5 == region.first );
		UNIT_ASSERT( m.Find(7, region) );UNIT_ASSERT( 5 == region.first );
		UNIT_ASSERT( !m.Find(8, region) );
		UNIT_ASSERT( !m.Find(9, region) );

		UNIT_ASSERT(m.AddRegion(8,9));
		UNIT_ASSERT(m.AddRegion(1,2));
		//state = [1,2], [5,7], [8,9], [10,10], [11,11]
		UNIT_ASSERT( m.Find(5, region) );UNIT_ASSERT( 5 == region.first );
		UNIT_ASSERT( m.Find(6, region) );UNIT_ASSERT( 5 == region.first );
		UNIT_ASSERT( m.Find(7, region) );UNIT_ASSERT( 5 == region.first );
		UNIT_ASSERT( m.Find(8, region) );UNIT_ASSERT( 8 == region.first );
		UNIT_ASSERT( m.Find(9, region) );UNIT_ASSERT( 8 == region.first );
		UNIT_ASSERT( m.Find(1, region) );UNIT_ASSERT( 1 == region.first );
		UNIT_ASSERT( m.Find(2, region) );UNIT_ASSERT( 1 == region.first );
		UNIT_ASSERT( !m.Find(3, region) );
		UNIT_ASSERT( !m.Find(13, region) );

		m.Clear();

		UNIT_ASSERT(m.AddRegion(8,9));
		UNIT_ASSERT(m.AddRegion(10,20));
		UNIT_ASSERT( m.Find(11, region) );UNIT_ASSERT( 10 == region.first );



	}

	void test2()
	{
		MapRegion m;
		UNIT_ASSERT(m.AddRegion(0,10));
		UNIT_ASSERT(m.AddRegion(11,20));
		UNIT_ASSERT(m.AddRegion(30,40));
		UNIT_ASSERT(m.AddRegion(50,60));


		//[0, 10][11, 20][30, 40][50, 60]
		MapRegion::VecSpan vec_span;
		m.GetSpan(1,3,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[0].lower && 2 == vec_span[0].dist);
		m.GetSpan(0,3,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[0].lower && 3 == vec_span[0].dist);
		m.GetSpan(1,10,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[0].lower && 9 == vec_span[0].dist);
		m.GetSpan(0,10,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[0].lower && 10 == vec_span[0].dist);
	
		//找不到的情况
		m.GetSpan(22,22,vec_span);
		UNIT_ASSERT(0 == vec_span.size());
		m.GetSpan(41,49,vec_span);
		UNIT_ASSERT(0 == vec_span.size());
		m.GetSpan(40, 49, vec_span);
		UNIT_ASSERT(0 == vec_span.size());

		//部分找不到，部分跨区间
		m.GetSpan(20,21,vec_span);
		UNIT_ASSERT(0 == vec_span.size());
		m.GetSpan(19,21,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(11 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.GetSpan(19,22,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(11 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.GetSpan(20,30,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.GetSpan(21,30,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[0].lower && 1 == vec_span[0].dist);


		//跨连续区间
		m.GetSpan(0,20,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[1].lower && 10 == vec_span[1].dist);
		UNIT_ASSERT(11 == vec_span[0].lower && 10 == vec_span[0].dist);
		m.GetSpan(1,20,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[1].lower && 9 == vec_span[1].dist);
		UNIT_ASSERT(11 == vec_span[0].lower && 10 == vec_span[0].dist);
		m.GetSpan(0,19,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[1].lower && 10 == vec_span[1].dist);
		UNIT_ASSERT(11 == vec_span[0].lower && 9 == vec_span[0].dist);
		m.GetSpan(10,11,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(11 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.GetSpan(9,11,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[1].lower && 1 == vec_span[1].dist);
		UNIT_ASSERT(11 == vec_span[0].lower && 1 == vec_span[0].dist);

		//[0, 10][11, 20][30, 40][50, 60]
		//跨不连续区间
		m.GetSpan(40,50,vec_span);
		UNIT_ASSERT(1 == vec_span.size());
		UNIT_ASSERT(50 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.GetSpan(39,50,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 1 == vec_span[0].dist);
		m.GetSpan(39,51,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 2 == vec_span[0].dist);
		m.GetSpan(39,60,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.GetSpan(39,61,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[1].lower && 1 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.GetSpan(29,61,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.GetSpan(20,61,vec_span);
		UNIT_ASSERT(2 == vec_span.size());
		UNIT_ASSERT(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.GetSpan(19,61,vec_span);
		UNIT_ASSERT(3 == vec_span.size());
		UNIT_ASSERT(11 == vec_span[2].lower && 1 == vec_span[2].dist);
		UNIT_ASSERT(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 11 == vec_span[0].dist);
		m.GetSpan(0,61,vec_span);
		UNIT_ASSERT(4 == vec_span.size());
		UNIT_ASSERT(0 == vec_span[3].lower && 10 == vec_span[3].dist);
		UNIT_ASSERT(11 == vec_span[2].lower && 10 == vec_span[2].dist);
		UNIT_ASSERT(30 == vec_span[1].lower && 11 == vec_span[1].dist);
		UNIT_ASSERT(50 == vec_span[0].lower && 11 == vec_span[0].dist);

	}	
	void test3()
	{
		MapRegion m;
		UNIT_ASSERT(m.AddRegion(11,20));
		UNIT_ASSERT(m.AddRegion(30,40));
		UNIT_ASSERT(m.AddRegion(50,60));

		MapRegion::VecSpan vec_span;
		m.GetSpan(1,3,vec_span);
		UNIT_ASSERT(0 == vec_span.size());
	}

	namespace
	{
		OverlapRegion g_c_region;

		void initComplexRegion()
		{
			g_c_region.clear();
			g_c_region.addRegion(5,20);
			g_c_region.addRegion(22,25);
			g_c_region.addRegion(15,23);
			g_c_region.addRegion(10,30);
			g_c_region.addRegion(10,20);
			g_c_region.addRegion(15,25);
		}
	}
	//测试点 和 简单区间
	void testOverlapRegion1()
	{
		OverlapRegion::VecLine vec_line;
		OverlapRegion region;
		region.addRegion(10,20);

		region.findOverLap(0, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		region.findOverLap(10, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(11, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(20, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(21, vec_line);
		UNIT_ASSERT(0 == vec_line.size());

		region.erase(10,15);

		region.findOverLap(0, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		region.findOverLap(10, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(11, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(20, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(21, vec_line);
		UNIT_ASSERT(0 == vec_line.size());

		region.erase(10,20);

		region.findOverLap(11, vec_line);
		UNIT_ASSERT(0 == vec_line.size());

	}
	//测试点 和 复杂区间
	void testOverlapRegion2()
	{
		OverlapRegion::VecLine vec_line;

		g_c_region.findOverLap(4, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		g_c_region.findOverLap(5, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		UNIT_ASSERT(5 == vec_line[0].start && 20 == vec_line[0].end);

		g_c_region.findOverLap(9, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		g_c_region.findOverLap(10, vec_line);
		UNIT_ASSERT(3 == vec_line.size());
		g_c_region.findOverLap(14, vec_line);
		UNIT_ASSERT(3 == vec_line.size());
		g_c_region.findOverLap(15, vec_line);
		UNIT_ASSERT(5 == vec_line.size());
		g_c_region.findOverLap(19, vec_line);
		UNIT_ASSERT(5 == vec_line.size());
		g_c_region.findOverLap(20, vec_line);
		UNIT_ASSERT(5 == vec_line.size());
		g_c_region.findOverLap(21, vec_line);
		UNIT_ASSERT(3 == vec_line.size());
		g_c_region.findOverLap(22, vec_line);
		UNIT_ASSERT(4 == vec_line.size());
		g_c_region.findOverLap(23, vec_line);
		UNIT_ASSERT(4 == vec_line.size());
		g_c_region.findOverLap(25, vec_line);
		UNIT_ASSERT(3 == vec_line.size());
		g_c_region.findOverLap(26, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		g_c_region.findOverLap(30, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		g_c_region.findOverLap(31, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		g_c_region.findOverLap(331, vec_line);
		UNIT_ASSERT(0 == vec_line.size());

	}
	//测试区间和简单区间
	void testOverlapRegion3()
	{

		OverlapRegion::VecLine vec_line;
		OverlapRegion region;
		region.addRegion(10,20);
		region.addRegion(22,30);

		region.findOverLap(0, 9, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		region.findOverLap(0, 10, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(0, 20, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(0, 21, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(0, 22, vec_line);
		UNIT_ASSERT(2 == vec_line.size());
		region.findOverLap(0, 29, vec_line);
		UNIT_ASSERT(2 == vec_line.size());
		region.findOverLap(0, 30, vec_line);
		UNIT_ASSERT(2 == vec_line.size());
		region.findOverLap(0, 31, vec_line);
		UNIT_ASSERT(2 == vec_line.size());

		region.findOverLap(31, 0, vec_line);
		UNIT_ASSERT(0 == vec_line.size());

		region.findOverLap(10, 10, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		region.findOverLap(11, 11, vec_line);
		UNIT_ASSERT(1 == vec_line.size());


	}
	//测试区间和复杂区间
	void testOverlapRegion4()
	{
		OverlapRegion::VecLine vec_line;

		g_c_region.findOverLap(4,3, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		g_c_region.findOverLap(4,4, vec_line);
		UNIT_ASSERT(0 == vec_line.size());
		g_c_region.findOverLap(3,4, vec_line);
		UNIT_ASSERT(0 == vec_line.size());

		g_c_region.findOverLap(4,5, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		g_c_region.findOverLap(4,9, vec_line);
		UNIT_ASSERT(1 == vec_line.size());
		g_c_region.findOverLap(4,10, vec_line);
		UNIT_ASSERT(3 == vec_line.size());
		g_c_region.findOverLap(4,15, vec_line);
		UNIT_ASSERT(5 == vec_line.size());
		g_c_region.findOverLap(4,20, vec_line);
		UNIT_ASSERT(5 == vec_line.size());
		g_c_region.findOverLap(4,22, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(4,23, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(4,25, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(4,29, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(4,30, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(4,31, vec_line);
		UNIT_ASSERT(6 == vec_line.size());

		g_c_region.findOverLap(10,25, vec_line);
		UNIT_ASSERT(6 == vec_line.size());

		g_c_region.findOverLap(15,23, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(20,22, vec_line);
		UNIT_ASSERT(6 == vec_line.size());
		g_c_region.findOverLap(21,22, vec_line);
		UNIT_ASSERT(4 == vec_line.size());
		g_c_region.findOverLap(21,21, vec_line);
		UNIT_ASSERT(3 == vec_line.size());

	}
}//namespace{



UNITTEST(region)
{
	initComplexRegion();
	test1();
	test2();
	test3();
	testOverlapRegion1();
	testOverlapRegion2();
	testOverlapRegion3();
	testOverlapRegion4();
}