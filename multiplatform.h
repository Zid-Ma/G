#pragma once
#if true
	//调用WindowsAPI
	//--------------
	#include <string>
	#include <fstream>
	#include <sstream>
	#include <iostream>
	#include <filesystem>
	#include <vector>
	#include <thread>
	#include <bitset>
	#include <wchar.h>
#else

#endif

#if true
using namespace std;
static class Print
{
private:
	string reset = "\033[0m";
	string blackred = "\033[41m";

	string red = "\033[31m";
	string green = "\033[32m";
public:
	Print(string s)
	{
		Line(s);
	}

	static void Line(string s)
	{
		try
		{
			cout << s << endl;
		}
		catch (exception e)
		{
			printf("Line打印出现错误");
		}
	}

	static void Debug(string s)
	{
		//return ;
		try 
		{
			HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			//绿色
			SetConsoleTextAttribute(outHandle, 0x02);
			cout << "Debug:" << s << endl;
			//count << "\033[32mDebug:" << s  << "\033[0m" << endl;
		}
		catch (exception e)
		{
			printf("Debug打印出现错误");
		}
	}

	static void Exception(string s)
	{
		try
		{
			/*
				代号	颜色
				0	黑色
				1	蓝色
				2	绿色
				3	浅绿色
				4	红色
				5	紫色
				6	黄色
				7	白色
				8	灰色
				9	淡蓝色
				A / a	淡绿色
				B / b	淡浅绿色
				C / c	淡红色
				D / d	淡紫色
				E / e	淡黄色
				F / f	亮白色
			*/
			HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			//红色
			SetConsoleTextAttribute(outHandle, 0x04);
			//system("color c");
			cout << "Exception:" << s << endl;
			if (false)
			{
				cout << "Please wait 3 second......" << endl;
				Sleep(1000);//睡眠3秒
				cout << "Please wait 2 second...." << endl;
				Sleep(1000);//睡眠3秒
				cout << "Please wait 1 second.." << endl;
				Sleep(1000);//睡眠3秒
			}
			//system("color a");
		}
		catch (exception e)
		{
			printf("Exception打印出现错误");
		}
	}
};

#else

#endif

static string GetEnter()
{
	return "\r\n";
}