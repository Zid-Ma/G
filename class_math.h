#pragma once
static class Math 
{
public:
	//获得两点之间的距离(距离不为负数)
	static float Distance(float _a, float _b)
	{
		if (_b - _a >= 0)
		{
			return _b - _a;
		}
		else 
		{
			return _a - _b;
		}
	}

	//获得两点之间最远轴的距离
	static float Distance_max(glm::vec3 _a, glm::vec3 _b)
	{
		int _x = Distance(_b.x , _a.x);
		int _y = Distance(_b.y , _a.y);
		int _z = Distance(_b.z , _a.z);
		if (_x > _y && _x > _z)
			return _x;
		else if (_y > _x && _y > _z)
			return _y;
		else
			return _z;
	}
public:
	static float Center(float _a, float _b)
	{
		if (_a >= _b)
		{
			return _b + (Distance(_a, _b) * 0.5f);
		}
		else
		{
			return _a + (Distance(_a, _b) * 0.5f);
		}
	}

	static glm::vec3 Center(glm::vec3 _a, glm::vec3 _b)
	{
		return glm::vec3(Center(_a.x, _b.x), Center(_a.y, _b.y), Center(_a.z, _b.z));
	}
public:
	//获得绝对值
	static float Abslution(float _a)
	{
		if (_a >= 0)
			return _a;
		else
			return _a * -1;
	}
};