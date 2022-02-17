#pragma once
static class TextSystem
{
private:
	static int lengthCount;//文本贴图的长度的个数
	static int widthCount;//文本贴图的宽度的个数
	static string mapPath;//文本贴图的路径(包括文件名)
	static string mapPath_S;
public:
	static string MapPath()
	{
		return mapPath;
	}
	static string MapPath_S()
	{
		return mapPath_S;
	}
public:
	static float Length()
	{
		return 1.0f / lengthCount;
	}
	static float Width()
	{
		return 1.0f / widthCount;
	}
public:
	TextSystem();

	TextSystem(int _lengthCout, int _widthCout)
	{
		lengthCount = _lengthCout;
		widthCount = _widthCout;
	}

	//获取字符在贴图的行位置
	static int LengthCount(char c)
	{
		//获取字符的ASCII码
		int i = c;
		int cout = i / widthCount;
		return cout;
	}

	//获取字符在贴图的列位置
	static int WidthCount(char c)
	{
		//获取字符的ASCII码
		int i = c;
		int cout = i % widthCount;
		return cout;
	}

	static int LengthCount(string s)
	{
		const char* c = s.c_str();
		int i = c[0];
		//如果i小于0，则说明string为全角字符
		if (i < 0)
		{
			const TCHAR tc = c[0] + c[1];
			int ti = tc;
			int cout = ti / widthCount;
			return cout;
		}
		else
		{
			int cout = i / widthCount;
			return cout;
		}
	}

	static int WidthCount(string s)
	{
		const char* c = s.c_str();
		int i = c[0];
		//如果i小于0，则说明string为全角字符
		if (i < 0)
		{
			const TCHAR tc = c[0] + c[1];
			int ti = tc;
			int cout = ti % widthCount;
			return cout;
		}
		else
		{
			int cout = i % widthCount;
			return cout;
		}
	}
};
int TextSystem::lengthCount = 12;
int TextSystem::widthCount = 12;
string TextSystem::mapPath = "text.png";
string TextSystem::mapPath_S = "text.png";

class Word:TextSystem
{
private:
	string Type = "";//字体类型
	string s = "";//字符串
public:
	Word()
	{

	}

	void Draw(string _s)
	{

	}
};