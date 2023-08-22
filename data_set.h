#pragma once
static class Settings
{
private:
	string debug = "���ã�";
	string path = Path_Resources + "settings/";
	string basicpath = path + "basic.txt";
	string gamepath = path + "game.txt";
private:
	vector<int*> sets;
	vector<string> setnames;
public:
	Settings()
	{
		data_initialization();
	}
	Settings(string _path)
	{
		path = _path;
		data_initialization();
	}
	~Settings()
	{
		Write_Game();
		Delete();
	}
public:
	void Confirm()
	{
		data_set();
	}
	void Cancel()
	{
		data_reset();
	}
	void ChangeData(string _name, int _i)
	{
		*find_data(_name) = _i;
	}
public:
	//��ȡ��Ӧ����
	void Read()
	{
		path = Path_Resources + "settings/";
		basicpath = path + "basic.txt";
		gamepath = path + "game.txt";
		Read(basicpath);
		Read_Game(gamepath);
	}
	void Write()
	{
		Write(basicpath);
	}
	void Write_Game()
	{
		Write_Game(gamepath);
	}
public:
	void Read(string _path)
	{
		Print::Debug("��ȡ�����ļ�:" + _path);
		//���ж�ȡ
		ifstream infile;
		infile.open(_path.data());   //���ļ����������ļ��������� 
		assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		string s = "";
		bool active_wirte = false;
		//Delete();
		while (getline(infile, s))
		{
			s = UTF8ToGB(s.c_str());
			//count << s << endl;
			if (active_wirte)
			{
				if (data(s))
				{
					active_wirte = false;
				}
			}
			else if (line(s))
			{
				active_wirte = true;
			}
		}
		infile.clear();
		infile.close();             //�ر��ļ������� 
	}
	void Read_Game(string _path)
	{
		//Print::Debug("��ȡ�����ļ�:" + _path);
		//���ж�ȡ
		ifstream infile;
		infile.open(_path.data());   //���ļ����������ļ��������� 
		assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
		string s = "";
		bool active_wirte = false;
		//Delete();
		while (getline(infile, s))
		{
			s = UTF8ToGB(s.c_str());
			//count << s << endl;
if (active_wirte)
{
	if (data(s))
	{
		active_wirte = false;
	}
}
else if (line_game(s))
{
	active_wirte = true;
}
		}
		infile.clear();
		infile.close();             //�ر��ļ������� 
	}
	void Write(string _path)
	{
		ofstream out(_path);
		string _s;
		_s = "#XYKS�Զ��������ļ�" + GetEnter();
		for (int i = 0; i < sets.size(); i++)
		{
			_s.append(setnames[i]);
			_s.append(" ");
			_s.append(to_string(*sets[i]));
			_s.append(GetEnter());
		}
		out << _s;
	}
	void Write_Game(string _path)
	{
		ofstream out(_path);
		string _s;
		_s = "#XYKS�Զ��������ļ�" + GetEnter();
		string _names[] = { "Plant_Score", "StoryLine"};
		string _datas[] = { to_string(Get_Plant_Score()), to_string(StoryLine) };
		for (int i = 0; i < 1; i++)
		{
			_s.append(_names[i]);
			_s.append(" ");
			_s.append(_datas[i]);
			_s.append(GetEnter());
		}
		out << _s;
	}
private:
	bool data(string _s)
	{
		if (_s.size() < 1)
		{
			return true;
		}
		else if (_s.substr(0, 1) == "#")
		{
			return true;
		}
		else if (_s.substr(0, 1) != "	")
		{
			return true;
		}
		return true;
	}
	bool line(string _s)
	{
		if (_s.size() < 1 || _s.size() < 3)
		{
			return false;
		}
		else if (_s.substr(0, 1) == "#")
		{
			return false;
		}
		for (int i = 0; i < sets.size(); i++)
		{
			if (_s.substr(0, setnames[i].size()) == setnames[i])
			{
				//Print::Debug(_s.substr(0, setnames[i].size()));
				*sets[i] = atoi(_s.substr(setnames[i].size() + 1).c_str());
				Print::Debug(setnames[i] + ":" + to_string(*sets[i]));
			}
		}
		return false;
	}
	bool line_game(string _s)
	{
		if (_s.size() < 1 || _s.size() < 3)
		{
			return false;
		}
		else if (_s.substr(0, 1) == "#")
		{
			return false;
		}
		for (int i = 0; i < sets.size(); i++)
		{
			if (_s.substr(0, setnames[i].size()) == "Plant_Score")
			{
				Set_Plant_Score(atoll(_s.substr(setnames[i].size() + 1).c_str()));
			}
			else if (_s.substr(0, setnames[i].size()) == "StoryLine")
			{
				StoryLine = atoll(_s.substr(setnames[i].size() + 1).c_str());
			}
		}
		return false;
	}
private:
	void data_set()
	{
		Language = *find_data("language");
		switch (*find_data("resolution"))
		{
		case -1:
			SCR_WIDTH = 1920;
			SCR_HEIGHT = 1080;
			break;
		case 0:
			SCR_WIDTH = 1024;
			SCR_HEIGHT = 768;
			break;
		case 1:
			SCR_WIDTH = 2556;
			SCR_HEIGHT = 1440;
			break;
		case 2:

			break;
		case 3:

			break;
		default:
			break;
		}
		ActiveLight = *find_data("illumination");
		Active_Shadow = *find_data("shadow");
		switch (*find_data("multisample"))
		{
		case -1:
			Multisampling = 16;
			break;
		case 0:
			Multisampling = 0;
			break;
		case 1:
			Multisampling = 2;
			break;
		case 2:
			Multisampling = 4;
			break;
		case 3:
			Multisampling = 8;
			break;
		case 4:
			Multisampling = 16;
			break;
		}
		Active_ssao = *find_data("ssao");
		switch (*find_data("fps"))
		{
		case -1:
			FramesPerSecondX = -1;
			break;
		case 0:
			FramesPerSecondX = 30;
			break;
		case 1:
			FramesPerSecondX = 60;
			break;
		case 2:
			FramesPerSecondX = 144;
			break;
		case 3:
			FramesPerSecondX = 240;
			break;
		case 4:
			FramesPerSecondX = 0;
			break;
		}
		FramesPerSecondDisplay = *find_data("fpsdisplay");
		Active_blend = *find_data("blend");
		Active_cull_face = *find_data("cullface");
		Active_srgb = *find_data("srgb");
	}
	void data_reset()
	{

	}
	int* find_data(string _name)
	{
		for (int i = 0; i < setnames.size(); i++)
		{
			if (setnames[i] == _name)
			{
				return sets[i];
			}
		}
		Print::Debug(debug);
		Print::Debug("δ�ҵ���Ӧ����");
		return NULL;
	}
	void data_initialization()
	{
		int _length = 11;
		string _name[] = {"language", "resolution", "illumination","shadow","multisample","ssao","fps","fpsdisplay","blend","cullface","srgb"};
		int* x;
		for (int i = 0; i < _length; i++)
		{
			x = new int(-1);
			sets.push_back(x);
			setnames.push_back(_name[i]);
		}
	}
private:
	void Delete()
	{
		Print::Debug("ɾ����̬������Settings��Ϣ...");
		int* x;
		if (sets.size() > 0)
		{
			for (int i = 0; i < sets.size(); i++)
			{
				Print::Debug(to_string(i));
				x = sets[i];
				if(x != NULL)
					delete x;
				sets[i] = NULL;
			}
			sets.clear();
		}
		Print::Debug("ɾ����̬������Settings��Ϣ:���");
	}
};
//����
static Settings Set;