#pragma once
static class TextSystem
{
private:
	static int lengthCount;//�ı���ͼ�ĳ��ȵĸ���
	static int widthCount;//�ı���ͼ�Ŀ�ȵĸ���
	static string mapPath;//�ı���ͼ��·��(�����ļ���)
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

	//��ȡ�ַ�����ͼ����λ��
	static int LengthCount(char c)
	{
		//��ȡ�ַ���ASCII��
		int i = c;
		int cout = i / widthCount;
		return cout;
	}

	//��ȡ�ַ�����ͼ����λ��
	static int WidthCount(char c)
	{
		//��ȡ�ַ���ASCII��
		int i = c;
		int cout = i % widthCount;
		return cout;
	}

	static int LengthCount(string s)
	{
		const char* c = s.c_str();
		int i = c[0];
		//���iС��0����˵��stringΪȫ���ַ�
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
		//���iС��0����˵��stringΪȫ���ַ�
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
	string Type = "";//��������
	string s = "";//�ַ���
public:
	Word()
	{

	}

	void Draw(string _s)
	{

	}
};