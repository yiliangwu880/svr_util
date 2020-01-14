/*
brief:������ҹ���
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
������ڸ��ӣ�������������䣬��set��ʵ�ִ���
������ num-1/distance ��õ�key����λ���ٶȸ���

*/


/*
���ص�����
����
	���䣺��ʾ������ []
	����ֵ��1��ʼ��0ֵ��ʾ��Ч
	����֮�䲻�����ص�
	
*/
class MapRegion
{
public:
	typedef std::map<uint32, uint32> LowMapUp;		//�����䣬��ֵΪkey,��ֵΪvalue	
	typedef std::pair<uint32, uint32> Region;
	//��ʾ��ĳ����Ŀ��
	struct Span
	{
		uint32 lower;  //�����ֵ
		uint32 dist;  //������
	};
	typedef std::vector<Span> VecSpan;

private:
	LowMapUp m_map_region;


public:
	bool AddRegion(uint32 lower, uint32 upper);
	//��ȡnum����������
	bool Find(uint32 num, Region &region) const;

	//����(lower, upper] �� �������������������򼯺ϡ�ֻ�ܻ�ȡÿ�����ϵĿ�ȡ�
	//vec_span.size() ==0 ��ʾʧ��
	void GetSpan(uint32 lower, uint32 upper, VecSpan &vec_span) const;

	//������������ֵ
	bool Erase(uint32 lower);
	void Clear();

};

/*
���ص�����
����
	�������ʾ
	����֮������ص�
	�㷨�ο���ͼͼ��

				��-----�ڵ�------------��
		��-------------�ڵ�------��
			��----------�㣨�ڵ㣩
--------------------------------------------------
	ÿ���㶼��һ��key, ��������ṹ��ÿ���ڵ㱣��һ���ߵ���Ϣ��
	����ĳ��������ص��ķ���������ĳ�����������������нڵ���Ϣ����֪������Щ�ߺ��Լ��ص���
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
	//���غ͵��ص�������
	void findOverLap(uint32 point, VecLine &vec_line) const;
	void findOverLap(uint32 start, uint32 end, VecLine &vec_line) const;
	bool erase(uint32 start, uint32 end);
	void clear();	

private:
	bool findRegion(uint32 start, uint32 end) const;
	//��û�������Լ�Ϊ�˵����Ϊ�յ�
	bool isEmptyPoint(const PointMapLines::iterator &point_it) const;

private:
	PointMapLines m_point_m_lines;
};