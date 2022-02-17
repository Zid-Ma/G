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
		return ;
		try 
		{
			cout << "Debug:" << s << endl;
			//cout << "\033[32mDebug:" << s  << "\033[0m" << endl;
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
			cout << "Exception:" << s << endl;
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