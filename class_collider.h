#pragma once
class Collider
{
protected:
	//vec3 point;
public:
	Collider()
	{
	}
};

class Collider_Sphere :Collider
{
protected:
	vec3 point;
	float radius;
public:
	string type;
	string name;
public:
	Collider_Sphere();

	Collider_Sphere(vec3 point_, float radius_)
	{
		point = point_;
		radius = radius_;
	}

public:
	bool Collision(Collider_Sphere* newColliderS)
	{
		//计算两点的距离
		float point_dis = glm::distance(Point(), newColliderS->Point());
		float radius_sum = radius + newColliderS->Radius();
		//判断半径之和是否小于两点间的距离,如果小于则碰撞
		if (radius_sum < point_dis)
		{
			return true;
		}
		return false;
	}

	static bool Collision(Collider_Sphere* newCS, Collider_Sphere* newCS1)
	{
		float point_dis = glm::distance(newCS->Point(), newCS1->Point());
		float radius_sum = newCS->Radius() + newCS1->Radius();
		if (radius_sum < point_dis)
		{
			return true;
		}
		return false;
	}

	//计算两点之间的距离
	float Distance(Collider_Sphere* newColliderS)
	{
		float point_dis = glm::distance(Point(), newColliderS->Point());
		return point_dis;
	}

	//属性
public:
	float Radius()
	{
		return radius;
	}

	void Radius(float radius_)
	{
		radius = radius_;
	}

	vec3 Point()
	{
		return point;
	}

	void Point(vec3 point_)
	{
		point = point_;
	}
};

class Collider_Cube :Collider
{
protected:
	vec3 point;
	float widthCout;
public:
	string type;
	string name;
public:
	Collider_Cube();

	Collider_Cube(vec3 point_, float width_)
	{
		point = point_;
		widthCout = width_;
	}

public:
	float Distance(Collider_Cube* newColliderC)
	{

	}

	//属性
public:
	float Width()
	{
		return widthCout;
	}

	void Radius(float width_)
	{
		widthCout = width_;
	}

	vec3 Point()
	{
		return point;
	}

	void Point(vec3 point_)
	{
		point = point_;
	}
};

class Collider_Coboid :Collider
{
protected:
	vec3 point;
	vec3 lwh;
public:
	string type;
	string name;
public:
	Collider_Coboid();

	Collider_Coboid(vec3 point_, vec3 lwh_)
	{
		lwh = lwh_;
		point = point_;
	}

	Collider_Coboid(vec3 point_, float length_x, float width_y, float height_z)
	{
		point = point_;
		lwh.x = length_x;
		lwh.y = width_y;
		lwh.z = height_z;
	}

public:
	//判断是否碰撞
	bool Collision(Collider_Coboid* newColliderCo)
	{
		if (newColliderCo != NULL)
		{
			//获得不同轴向间的距离
			float dis_x = glm::distance(point.x, newColliderCo->Point().x);
			float dis_y = glm::distance(point.y, newColliderCo->Point().y);
			float dis_z = glm::distance(point.z, newColliderCo->Point().z);
			vec3 len = (lwh + newColliderCo->LWH());
			vec3 dis = vec3(dis_x * 2, dis_y * 2, dis_z * 2);
			//Print::Debug(to_string(len.y) + "X" + to_string(newColliderCo->Point().y));
			if (dis.x < len.x && dis.y < len.y && dis.z < len.z)
			{
				return true;
			}
		}
		return false;
	}
	static bool Collision(Collider_Coboid* newCCo, Collider_Coboid* newCCo1)
	{
		if (newCCo != NULL && newCCo1 != NULL)
		{
			//获得不同轴向间的距离
			float dis_x = glm::distance(newCCo->Point().x, newCCo1->Point().x);
			float dis_y = glm::distance(newCCo->Point().y, newCCo1->Point().y);
			float dis_z = glm::distance(newCCo->Point().z, newCCo1->Point().z);
			vec3 len = (newCCo->LWH() + newCCo1->LWH());
			vec3 dis = vec3(dis_x * 2, dis_y * 2, dis_z * 2);
			//Print::Debug(to_string(len.x) + "X" + to_string(newCCo->Point().x) + "X" + to_string(dis_x) + "X" + to_string(newCCo1->Point().x));
			//Print::Debug(to_string(len.y) + "Y" + to_string(newCCo->Point().y) + "Y" + to_string(dis_y) + "Y" + to_string(newCCo1->Point().y));
			//Print::Debug(to_string(len.z) + "Z" + to_string(newCCo->Point().z) + "Z" + to_string(dis_z) + "Z" + to_string(newCCo1->Point().z));
			if (dis.x < len.x && dis.y < len.y && dis.z < len.z)
			{
				//Print::Debug("true");
				return true;
			}
		}
		return false;
	}
	float Distance(Collider_Coboid* newColliderCo)
	{
		float point_dis = glm::distance(Point(), newColliderCo->Point());
		return point_dis;
	}

	//属性
public:
	vec3 LWH()
	{
		return lwh;
	}

	float Length()
	{
		return lwh.x;
	}

	void Length(float length_)
	{
		lwh.x = length_;
	}

	float Width()
	{
		return lwh.y;
	}

	void Width(float width_)
	{
		lwh.y = width_;
	}

	float Height()
	{
		return lwh.z;
	}

	float Height(float height_)
	{
		lwh.z = height_;
	}

	vec3 Point()
	{
		return point;
	}

	void Point(vec3 point_)
	{
		point = point_;
	}
};


class Collider_Polygon :Collider
{
protected:
	vec3 point;
	float widthCout;
public:
	string type;
	string name;
public:
	Collider_Polygon(vec3 point_, float width_)
	{
		point = point_;
		widthCout = width_;
	}

public:
	float Distance(Collider_Polygon* newColliderC)
	{

	}

	//属性
public:
	float Width()
	{
		return widthCout;
	}

	void Width(float width_)
	{
		widthCout = width_;
	}

	vec3 Point()
	{
		return point;
	}

	void Point(vec3 point_)
	{
		point = point_;
	}
};

class CheckCollider :Collider 
{
public:
	static bool Collision(Collider_Coboid* nCCo, Collider_Sphere* nCS)
	{
		return Collision(nCS, nCCo);
	}

	static bool Collision(Collider_Sphere* nCS, Collider_Coboid* nCCo)
	{
		//获取圆到长方体的最近点
		{
			//中心点之间的距离
			float dis = glm::distance(nCS->Point(), nCCo->Point());
			//获取中心点的各各轴的距离
			vec3 point_dis = Distance(nCS->Point(), nCCo->Point());
			vec3 radius_length_dis = vec3(nCS->Radius() + nCCo->Length(), nCS->Radius() + nCCo->Width(), nCS->Radius() + nCCo->Height());
			vec3 difference = point_dis - radius_length_dis;
			//排序出除去边长、半径距离后距离最短的轴向
			vec3 min_axis_sort = Sort_MiniAxis(difference);
			//如果不为0,证明该面可以作为检测面
			if (difference[min_axis_sort[0]] > 0)
			{
				vec3 v;
				//当3个轴平面都未相交时，判断长方体顶点位置
				//判断圆顶点位于长方体各轴的正极还是负级
				for (int i = 0; i < 3; i++)
				{
					if (nCS->Point()[i] > nCCo->Point()[i])
					{
						//正极
						v[i] = nCCo->Point()[i] + (nCCo->LWH()[i] * 0.5f);
					}
					else
					{
						//负极
						v[i] = nCCo->Point()[i] - (nCCo->LWH()[i] * 0.5f);
					}
				}
				//判断圆心到顶点的距离
				float dis_ = glm::distance(nCS->Point(), v);
				//如果距离小于半径则相交
				if (dis_ < nCS->Radius())
					return true;
			}
			else if (difference[min_axis_sort[1]] > 0)
			{
				vec3 v;
				//当只有两个轴平面未相交时
				//判断圆顶点位于长方体各轴的正极还是负级
				for (int i = 0; i < 3; i++)
				{
					//只判断未相交轴向
					if (i != min_axis_sort[0])
					{
						if (nCS->Point()[i] > nCCo->Point()[i])
						{
							//正极
							v[i] = nCCo->Point()[i] + (nCCo->LWH()[i] * 0.5f);
						}
						else
						{
							//负极
							v[i] = nCCo->Point()[i] - (nCCo->LWH()[i] * 0.5f);
						}
					}
				}
				v[min_axis_sort[0]] = nCCo->Point()[min_axis_sort[0]] + (nCS->Point()[min_axis_sort[0]] - nCCo->Point()[min_axis_sort[0]]);
				float dis_ = glm::distance(nCS->Point(), v);
				if (dis_ < nCS->Radius())
					return true;
			}
			else if (difference[min_axis_sort[2]] > 0)
			{
				vec3 v;
				//当只有一个轴平面未相交时
				//判断圆顶点位于长方体各轴的正极还是负级
				for (int i = 0; i < 3; i++)
				{
					//只判断未相交轴向
					if (i != min_axis_sort[0] && i != min_axis_sort[1])
					{
						if (nCS->Point()[i] > nCCo->Point()[i])
						{
							//正极
							v[i] = nCCo->Point()[i] + (nCCo->LWH()[i] * 0.5f);
						}
						else
						{
							//负极
							v[i] = nCCo->Point()[i] - (nCCo->LWH()[i] * 0.5f);
						}
					}
				}
				v[min_axis_sort[0]] = nCCo->Point()[min_axis_sort[0]] + (nCS->Point()[min_axis_sort[0]] - nCCo->Point()[min_axis_sort[0]]);
				v[min_axis_sort[1]] = nCCo->Point()[min_axis_sort[1]] + (nCS->Point()[min_axis_sort[1]] - nCCo->Point()[min_axis_sort[1]]);
				float dis_ = glm::distance(nCS->Point(), v);
				if (dis_ < nCS->Radius())
					return true;
			}
			else
			{
				//当3个轴的的差都小于0时，证明物体相交
				return true;
			}
			return false;
		}
	}

	static vec3 Distance(vec3 v1, vec3 v2)
	{
		vec3 dis;
		dis.x = glm::distance(v1.x, v2.x);
		dis.y = glm::distance(v1.y, v2.y);
		dis.z = glm::distance(v1.z, v2.z);
		return dis;
	}

	static vec3 Sort_MiniAxis(vec3 v)
	{
		vec3 nv;
		{
			if (v.x <= v.y && v.x <= v.z)
			{
				nv[0] = 0;
			}
			else if (v.y <= v.x && v.y <= v.z)
			{
				nv[0] = 1;
			}
			else
				nv[0] = 2;
		}
		{
			if (v.x > v.y && v.x > v.z)
			{
				nv[2] = 0;
			}
			else if (v.y > v.x && v.y > v.z)
			{
				nv[2] = 1;
			}
			else
				nv[2] = 2;
		}
		{
			if (nv[0] != 0 && nv[2] != 0)
			{
				nv[1] = 0;
			}
			else if (nv[0] != 1 && nv[2] != 1)
			{
				nv[1] = 1;
			}
			else
				nv[2] = 2;
		}
		return nv;
	}
};
