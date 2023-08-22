#pragma once
string Get_Story(string _path)
{
	ifstream _in;
	_in.open(_path);
	if (_in.is_open())
	{
		string _ls = "";
		string _s = "";
		while (getline(_in, _ls))
		{
			//if (_s.size() > 150 || _s.length() > 100)
				//break;
			_s.append(_ls);
			_s.append("\n");
		}
		_in.clear();
		_in.close();
		return _s;
	}
	_in.clear();
	_in.close();
	Print::Exception("获取Story文本时失败");
	return "";
}

int StoryLine = 0;

void Init_StoryLine()
{

}
void ChangeStoryLine()
{
	string _ssstory = Get_Story(Path_Resources + "story/开始.txt");
	_ssstory = UTF8ToGB(_ssstory.c_str());
	Print::Line(_ssstory);
}