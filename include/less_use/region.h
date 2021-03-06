/*
brief:区间查找功能
use example:
	MapRegion m;
	m.addRegion(1,10);
	m.addRegion(2,20); //add fail

	Region region
	m.find(9,region); //region = [1,10]

*/
#pragma once
#include <list>
#include <map>
#include <vector>
#include <climits>
#include "typedef.h"

/*
好像过于复杂，如果是连续区间，用set来实现代替
或者用 num-1/distance 获得的key来定位，速度更快

*/


/*
不重叠区间
规则：
	区间：表示闭区间 []
	区间值从1开始，0值表示无效
	区间之间不可以重叠
	
*/
class MapRegion
{
public:
	typedef std::map<uint32, uint32> LowMapUp;		//闭区间，左值为key,右值为value	
	typedef std::pair<uint32, uint32> Region;
	//表示在某区间的跨度
	struct Span
	{
		uint32 lower;  //区间键值
		uint32 dist;  //区间跨度
	};
	typedef std::vector<Span> VecSpan;

private:
	LowMapUp m_map_region;


public:
	bool AddRegion(uint32 lower, uint32 upper);
	//获取num所属的区间
	bool Find(uint32 num, Region &region) const;

	//查找(lower, upper] 和 对象所有区间的相关区域集合。只能获取每个集合的跨度。
	//vec_span.size() ==0 表示失败
	void GetSpan(uint32 lower, uint32 upper, VecSpan &vec_span) const;

	//传入闭区间的左值
	bool Erase(uint32 lower);
	void Clear();

};

/*
可重叠区间
规则：
	闭区间表示
	区间之间可以重叠
	算法参考下图图：

				点-----节点------------点
		点-------------节点------点
			点----------点（节点）
--------------------------------------------------
	每个点都是一个key, 点是链表结构，每个节点保存一条线的信息。
	查找某点和区间重叠的方法：查找某点左右最近两点的所有节点信息，就知道有那些线和自己重叠。
*/
class OverlapRegion
{
public:
	struct Line 
	{
		uint32 start;
		uint32 end;
	};
	typedef std::vector<Line> VecLine;
	typedef std::list<Line> Lines;
	typedef std::map<uint32, Lines> PointMapLines;
public:
	bool addRegion(uint32 start, uint32 end);
	//返回和点重叠的区间
	void findOverLap(uint32 point, VecLine &vec_line) const;
	void findOverLap(uint32 start, uint32 end, VecLine &vec_line) const;
	bool erase(uint32 start, uint32 end);
	void clear();	

private:
	bool findRegion(uint32 start, uint32 end) const;
	//点没保存以自己为端点的线为空点
	bool isEmptyPoint(const PointMapLines::iterator &point_it) const;

private:
	PointMapLines m_point_m_lines;
};