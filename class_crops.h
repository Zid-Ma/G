#pragma once
//庄稼
class Crop
{
private:
	static bool first;
public:
	int stage;//当前成长阶段
	string stage_names[6] = {"","","","","",""};
	vector<ModelX*> model;
	static Shader shader;
public:
	static bool Existing[20 * 20];
public:
	Crop()
	{

	}
	~Crop()
	{
		for (int i = 0; i < model.size(); i++)
		{
			delete model[i];
		}
	}
	void First()
	{
		if (first)
		{
			shader = Shader(Path_Shader + "multiple.vert", Path_Shader + "font.frag");
			Load_shader(shader);
			//shader.setVec4("color", vec4(1.0, 1.0, 1.0, 1.0));
		}
	}
public:
	static void Existing_Add(int _count)
	{
		Existing[_count] = true;
	}
	static void Existing_Remove(int _count)
	{
		Existing[_count] = false;
	}
};
bool Crop::first = true;
bool Crop::Existing[20 * 20] = {false};
Shader Crop::shader;

class Potato:Crop
{
private:
	bool first = true;
public:
	Potato()
	{
		//First();
	}
	~Potato()
	{
		
	}
	void First()
	{
		if (first)
		{
			ModelX* ptr;
			ptr = new ModelX(Path_Model + "potato/土豆一X.txt");//potato/土豆一X.txt
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "potato/土豆二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "potato/土豆二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "potato/土豆二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "potato/土豆二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "potato/土豆二X.txt");
			model.push_back(ptr);
			stage = 0;
			stage_names[0] = "入土";
			stage_names[1] = "萌芽";
			stage_names[2] = "新生";
			stage_names[3] = "蓬发";
			stage_names[4] = "成熟";
			stage_names[5] = "老成";
			InitFont();
			first = false;
		}
	}
public:
	float stage_exp[6] = {10,100,200,400,1000,2000};
	vector<UI_Font> stage_fonts;
	void InitFont()
	{
		UI_Font u;
		u = New_UI_Font("",vec3(0,0,0),vec3(100,100,0),32,-1,NULL,stage_names[0],stage_names[1],stage_names[2],stage_names[3],stage_names[4],stage_names[5]);
		stage_fonts.push_back(u);
	}
	void Draw(vec3 _point)
	{
		First();
		_point.y += 0.5f;
		if(model.size() > stage + 1)
			model[stage]->Draw(&_point);
		switch (stage)
		{
		case 0:
			Square::Draw(stage_fonts[0].texture, _point, shader);
			//Print::Debug("第一阶段绘制");
			break;
		case 1:
			Square::Draw(stage_fonts[0].textures[0], _point, shader);
			//Print::Debug("第二阶段绘制");
			break;
		case 2:
			Square::Draw(stage_fonts[0].textures[1], _point, shader);
			//Print::Debug("第三阶段绘制");
			break;
		case 3:
			Square::Draw(stage_fonts[0].textures[2], _point, shader);
			break;
		case 4:
			Square::Draw(stage_fonts[0].textures[3], _point, shader);
			break;
		case 5:
			Square::Draw(stage_fonts[0].textures[4], _point, shader);
			break;
		}

		//Print::Debug(to_string(_point.x) + "?" + to_string(_point.y) + "?" + to_string(_point.z));
	}
	void Stage(int _stage)
	{
		stage = _stage;
	}
};

class Potatos :Potato
{
public:
	vector<int> stages;
	vector<vec3> collection;
	vector<float> time;//成长时间
public:
	Potatos()
	{
		
	}
	~Potatos()
	{

	}
public:
	void Draw()
	{
		if (collection.size() > 0)
		{
			for (int i = 0; i < collection.size(); i++)
			{
				Stage(stages[i]);
				Potato::Draw(collection[i]);
			}
		}
	}
	void FixedUpdate()
	{
		if (collection.size() > 0)
		{
			for (int i = 0; i < collection.size(); i++)
			{
				time[i] += 1;
				for (int ix = 0; ix < 6; ix++)
				{
					if (time[i] > stage_exp[ix])
					{
						stages[i] = ix;
						Set_Plant_Score(Get_Plant_Score() + 1);
						//Print::Debug("change stage" + to_string(ix));
					}
				}
				//Print::Debug(to_string(time[i]));
			}
		}
	}
};
static Potatos potatos;


class Cabbage :Crop
{
private:
	bool first = true;
public:
	Cabbage()
	{
		//First();
	}
	~Cabbage()
	{

	}
	void First()
	{
		if (first)
		{
			ModelX* ptr;
			ptr = new ModelX(Path_Model + "cabbage/白菜一X.txt");//potato/土豆一X.txt
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "cabbage/白菜二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "cabbage/白菜二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "cabbage/白菜二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "cabbage/白菜二X.txt");
			model.push_back(ptr);
			ptr = new ModelX(Path_Model + "cabbage/白菜二X.txt");
			model.push_back(ptr);
			stage = 0;
			stage_names[0] = "入土";
			stage_names[1] = "萌芽";
			stage_names[2] = "新生";
			stage_names[3] = "蓬发";
			stage_names[4] = "成熟";
			stage_names[5] = "老成";
			InitFont();
			first = false;
		}
	}
public:
	float stage_exp[6] = { 10,100,200,400,800,1600 };
	vector<UI_Font> stage_fonts;
	void InitFont()
	{
		UI_Font u;
		u = New_UI_Font("", vec3(0, 0, 0), vec3(100, 100, 0), 32, -1, NULL, stage_names[0], stage_names[1], stage_names[2], stage_names[3], stage_names[4], stage_names[5]);
		stage_fonts.push_back(u);
	}
	void Draw(vec3 _point)
	{
		First();
		_point.y += 0.5f;
		if (model.size() > stage + 1)
			model[stage]->Draw(&_point);
		switch (stage)
		{
		case 0:
			Square::Draw(stage_fonts[0].texture, _point, shader);
			//Print::Debug("第一阶段绘制");
			break;
		case 1:
			Square::Draw(stage_fonts[0].textures[0], _point, shader);
			//Print::Debug("第二阶段绘制");
			break;
		case 2:
			Square::Draw(stage_fonts[0].textures[1], _point, shader);
			//Print::Debug("第三阶段绘制");
			break;
		case 3:
			Square::Draw(stage_fonts[0].textures[2], _point, shader);
			break;
		case 4:
			Square::Draw(stage_fonts[0].textures[3], _point, shader);
			break;
		case 5:
			Square::Draw(stage_fonts[0].textures[4], _point, shader);
			break;
		}

		//Print::Debug(to_string(_point.x) + "?" + to_string(_point.y) + "?" + to_string(_point.z));
	}
	void Stage(int _stage)
	{
		stage = _stage;
	}
};

class Cabbages :Cabbage
{
public:
	vector<int> stages;
	vector<vec3> collection;
	vector<float> time;//成长时间
public:
	Cabbages()
	{

	}
	~Cabbages()
	{

	}
public:
	void Draw()
	{
		if (collection.size() > 0)
		{
			for (int i = 0; i < collection.size(); i++)
			{
				Stage(stages[i]);
				Cabbage::Draw(collection[i]);
			}
		}
	}
	void FixedUpdate()
	{
		if (collection.size() > 0)
		{
			for (int i = 0; i < collection.size(); i++)
			{
				time[i] += 1;
				for (int ix = 0; ix < 6; ix++)
				{
					if (time[i] > stage_exp[ix])
					{
						stages[i] = ix;
						Set_Plant_Score(Get_Plant_Score() + 1);
						//Print::Debug("change stage" + to_string(ix));
					}
				}
				//Print::Debug(to_string(time[i]));
			}
		}
	}
};
static Cabbages cabbages;

class Crops_Time
{
	static vector<void*> collection;
public:
	Crops_Time()
	{

	}
public:
	static void Draw()
	{
		potatos.Draw();
		cabbages.Draw();
	}
	static void FixedUpdate()
	{
		//设置：一秒钟更新一次
		for (int i = 0; i < collection.size(); i++)
		{
			switch (i)
			{
			case 0:
				((Potatos*)collection[i])->FixedUpdate();
				break;
			case 1:
				((Cabbages*)collection[i])->FixedUpdate();
			}
		}
	}
};
vector<void*> Crops_Time::collection = { &potatos, &cabbages };