#pragma once
class Collider_Layer
{
public:
	Collider_Layer()
	{

	}
	~Collider_Layer()
	{
		Delete_Collection();
	}
public:
	vector<string> names;
public:
	vector<vec3*> collection_point;//通常用于重力检测
	vector<Collider_Coboid*> collection_entity;//默认
	vector<Collider_Sphere*> collection_energy;//attack
	//添加一个碰撞体
	void Add(string _name, Collider_Coboid* _cc)
	{
		names.push_back(_name);
		collection_entity.push_back(_cc);
	}
public:
	//返回最近的一个碰撞体
	Collider_Coboid* Recent_Collision_Entity(vec3 _point)
	{
		int entity_Cout = collection_entity.size();
		//Print::Debug(to_string(entity_Cout));
		if (entity_Cout > 0)
		{
			int recent_i = 0;
			float mindistance = 1000.0f;
			for (int i = 0; i < entity_Cout; i++)
			{
				float cur_dis = glm::distance(collection_entity[i]->Point(), _point);
				if (mindistance > cur_dis)
				{
					recent_i = i;
					mindistance = cur_dis;
					//Print::Debug(to_string(mindistance));
				}
			}
			return collection_entity[recent_i];
		}
		else
		{
			return 0;
		}
	}

	//返回最近的多个碰撞体
	vector<Collider_Coboid*> Recent_Collision_Entity(vec3 _point, int _count)
	{
		int entity_Cout = collection_entity.size();
		//Print::Debug(to_string(entity_Cout));
		vector<Collider_Coboid*> ccos;//排序
		vector<Collider_Coboid*> nccos;
		//Print::Debug(to_string(entity_Cout));
		if (entity_Cout > 0 && _count > 0)
		{
			for (int i = 0; i < entity_Cout; i++)
			{
				ccos.push_back(collection_entity[i]);
			}
			//Print::Debug(to_string(ccos.size()));
			if (_count > entity_Cout)
				_count = entity_Cout;
			for (int i = 0; i < _count; i++)
			{
				int recent_i = 0;
				float mindistance = 1000.0f;
				for (int ii = 0; ii < ccos.size(); ii++)
				{
					float cur_dis = glm::distance(ccos[ii]->Point(), _point);
					if (mindistance > cur_dis)
					{
						recent_i = ii;
						mindistance = cur_dis;
						//Print::Debug(to_string(mindistance));
					}
				}
				nccos.push_back(ccos[recent_i]);
				//删除指定位置元素
				ccos.erase(ccos.begin() + recent_i);
				//Print::Debug(to_string(ccos.size()) + "X" + to_string(recent_i));
				if (ccos.size() <= 0)
					break;
			}
			return nccos;
		}
		else if (entity_Cout>0 && _count == 0)
		{
			nccos = collection_entity;
			return nccos;
		}
		else
		{
			return nccos;
		}
	}

	//返回最近的多个点碰撞体
	vector<vec3*> Recent_Collision_Point(vec3 _point, int _count)
	{
		int point_Count = collection_point.size();
		//Print::Debug(to_string(entity_Cout));
		vector<vec3*> ccos;//排序
		vector<vec3*> nccos;
		Print::Debug(to_string(point_Count));
		if (point_Count > 0 && _count > 0)
		{
			for (int i = 0; i < point_Count; i++)
			{
				ccos.push_back(collection_point[i]);
			}
			//Print::Debug(to_string(ccos.size()));
			if (_count > point_Count)
				_count = point_Count;
			for (int i = 0; i < _count; i++)
			{
				int recent_i = 0;
				float mindistance = 1000.0f;
				for (int ii = 0; ii < ccos.size(); ii++)
				{
					float cur_dis = glm::distance(*ccos[ii], _point);
					if (mindistance > cur_dis)
					{
						recent_i = ii;
						mindistance = cur_dis;
						//Print::Debug(to_string(mindistance));
					}
				}
				nccos.push_back(ccos[recent_i]);
				//删除指定位置元素
				ccos.erase(ccos.begin() + recent_i);
				//Print::Debug(to_string(ccos.size()) + "X" + to_string(recent_i));
				if (ccos.size() <= 0)
					break;
			}
			return nccos;
		}
		else if (point_Count > 0 && _count == 0)
		{
			nccos = collection_point;
			return nccos;
		}
		else
		{
			return nccos;
		}
	}

	void Delete_Collection()
	{
		for (int i = 0; i < collection_point.size(); i++)
		{
			vec3* CP = collection_point[i];
			delete CP;
		}
		collection_point.clear();
		for (int i = 0; i < collection_entity.size(); i++)
		{
			Collider_Coboid* CC = collection_entity[i];
			delete CC;
		}
		collection_entity.clear();
		//collection_entity.swap(collection_entity);
		for (int i = 0; i < collection_energy.size(); i++)
		{
			Collider_Sphere* CS = collection_energy[i];
			delete CS;
		}
		collection_energy.clear();
		//collection_energy.swap(collection_energy);
	}
};

static class Collider_Layers
{
public:
	Collider_Layers()
	{

	}
	~Collider_Layers()
	{
		Delete_Collider_Layers();
	}
public:
	static vector<string> Names;
	static vector<Collider_Layer*> Collection;
public:
	static Collider_Layer* Get(string _name)
	{
		for (int i = 0; i < Collection.size(); i++)
		{
			if (Names[i] == _name)
			{
				return Collection[i];
			}
		}
		return NULL;
	}
public:
	//碰撞体图层初始化
	static void Initialization_Collider_Layers(int _layerAmount)
	{
		for (int i = 0; i < _layerAmount; i++)
		{
			Collider_Layer* cl = new Collider_Layer();
			Collection.push_back(cl);
		}
	}
	static void Delete_Collider_Layers()
	{
		for (int i = 0; i < Collection.size(); i++)
		{
			delete Collection[i];
		}
	}
};
vector<string> Collider_Layers::Names;
vector<Collider_Layer*> Collider_Layers::Collection;