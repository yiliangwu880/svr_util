#include "su_include.h"

#include "str_util.h"
#include <cassert>
#include "unit_test.h"
using namespace std;
using namespace su;
namespace
{
	void test1()
	{
        string src;
        VecStr out;
        StrUtil::split(src, " ", out);
        assert(out.empty());

        src = "a";
        StrUtil::split(src, " ", out);
        assert(1 == out.size());
        assert("a"==out[0]);

        src = "a ";
        StrUtil::split(src, " ", out);
        assert(1 == out.size());
        assert("a"==out[0]);

        src = " a ";
        StrUtil::split(src, " ", out);
        assert(1 == out.size());
        assert("a"==out[0]);

        src = " a b";
        StrUtil::split(src, " ", out);
        assert(2 == out.size());
        assert("a"==out[0]);
        assert("b"==out[1]);

        src = " a b";
        StrUtil::split(src, " ", out, 1);
        assert(1 == out.size());
        assert("a"==out[0]);


	}
    void test2()
    {
        string s = "abckkabc";
        StrUtil::replace(s, "", "1");
        StrUtil::replace(s, 'a', '1');
        assert(s == "1bckk1bc");
        StrUtil::replace(s, "1bc", "123");
        assert(s == "123kk123");
        StrUtil::erase(s, "1bc");
        assert(s == "123kk123");
        StrUtil::erase(s, "123");
        assert(s == "kk");
        StrUtil::erase(s, "");
        assert(s == "kk");
        StrUtil::erase(s, "1");
        assert(s == "kk");

    }
	void test3()
	{
		int i = 3;
		string s = StrNum::NumToStr(i);
		UNIT_ASSERT(s == "3");
		s = StrNum::NumToStr(0.30f);
		uint64 bb = 6000009898983433555;
		s = StrNum::NumToStr(bb);
		UNIT_ASSERT(s == "6000009898983433555");
		bool r = false;
		s = StrNum::NumToStr(r);
		UNIT_ASSERT(s == "false");
		s = StrNum::NumToStr(!r);
		UNIT_ASSERT(s == "true");
		UNIT_ASSERT(StrNum::StrToNum<int>("34") == 34);
		UNIT_ASSERT(StrNum::StrToNum<double>("34") == 34);
		UNIT_ASSERT(StrNum::StrToNum<uint64>("6000009898983433555") == 6000009898983433555);
		StrNum::StrToNum<float>("34", &r);
		UNIT_ASSERT(r);
		StrNum::StrToNum<float>("a34", &r);
		UNIT_ASSERT(!r);


	}
	void test4()
	{//StrUtil::format

		string out, correct;
		string in = "12345";

		UNIT_ASSERT(in.length() == 5);

		StrFormat::format(out, "%s", in.c_str());
		UNIT_ASSERT(in == out);

		//StrUtil::format(out, "a%d%s", in.c_str()); //段错误 SIGSEGV      11    

		{
			string ret = StrFormat::format("%s", in.c_str());
			UNIT_ASSERT(in == ret);
		}
		{
			string ret = StrFormat::format("a%sd", in.c_str());
			UNIT_ASSERT(("a"+in+"d") == ret);
		}
		//生成大长度
		in.clear();
		for (int i=0; i<10000*100; ++i)
		{
			in.append("ab");
		}
		StrFormat::format(out, "abc_%s_end", in.c_str());
		correct = "abc_" + in + "_end";
		UNIT_ASSERT(correct == out);
	}

}//end namespace

UNITTEST(testStringTool)
{
	test1();
    test2();
	test3();
	test4();





}