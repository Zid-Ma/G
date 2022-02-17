#pragma once
#ifndef  _LOADOBJ_H_
#define  _LOADOBJ_H_
//
//#include <math.h>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>

using namespace std;

#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z*(v).z)  
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector  
#define norm(v)    sqrt(norm2(v))  // norm = length of vector  
#define d(u,v)     norm(u-v)       // distance = norm of difference  

// Obj loader
struct TriangleFace
{
	int v[3]; // vertex indices //顶点索引
	int t[3]; // 纹理对象索引
	int n[3]; //法向量对象索引
};

class float3
{
public:
	float x;
	float y;
	float z;
public:
	float3() { x = 0; y = 0; z = 0; }
	float3(float mx, float my, float mz) { x = mx; y = my; z = mz; }
	~float3() {}

	float3 operator+(float3);
	float3 operator-(float3);
	float3 operator/(float);

	friend float3 operator*(float m, float3 f3)
	{
		return float3(f3.x * m, f3.y * m, f3.z * m);
	}
	friend float3 operator*(float3 f3, float m)
	{
		return float3(f3.x * m, f3.y * m, f3.z * m);
	}

	float3 operator=(float3);

	float3& operator += (float3);

};

float3 float3::operator +(float3 m)
{
	float3 result;
	result.x = x + m.x;
	result.y = y + m.y;
	result.z = z + m.z;
	return result;
}

float3 float3::operator - (float3 m)
{
	float3 result;
	result.x = x - m.x;
	result.y = y - m.y;
	result.z = z - m.z;
	return result;
}

float3 float3::operator /(float m)
{
	if (m == 0) {
		printf("error /");
		return float3(x, y, z);
	}
	else
		return float3(x / m, y / m, z / m);
}

float3 float3::operator =(float3 f3)
{
	x = f3.x;
	y = f3.y;
	z = f3.z;
	return float3(x, y, z);
}

struct TriangleMesh
{
	vector<float3> verts;
	vector<vec2> textures;//texture_coordinates
	vector<vec3> normals;//normal vector
	vector<TriangleFace> faces;//三角形面顶点（索引）
	int faces_type;//索引对象类型(个数)
	//渲染属性文件名
	string material_mtl;
	//索引类
	string material_usemtl;
	string map_di;
	string map_sp;
	string map_no;
	string map_re;
	string path;
	//是否为碰撞体
	bool active_collider;
	//模型的包围盒
	float3 bounding_box[2];//纹理坐标
	//模型的包围球参数
	float3 bounding_sphere_c;//法向量
	float bounding_sphere_r;//是否有光滑组
};

//TriangleMesh _mesh;

int total_number_of_triangles = 0;

// Scene bounding box 
float3 scene_aabbox_min;
float3 scene_aabbox_max;

//加载OBJ文件对象,记得要delete掉mesh
vector<TriangleMesh*> loadObj(const std::string filename, TriangleMesh& _mesh);
void loadObjX(const std::string filename, vector<TriangleMesh*>& _meshs);
vector<TriangleMesh*> loadMtl(const string filename, TriangleMesh& _mesh);
void loadMtlX(const string filename, vector<TriangleMesh*>& _meshs);

std::string Get_Xpath(std::string path)
{
	//获取到最后一个斜杠的位置
	int a = 0, b = 0;
	for (int i = 0; i < path.length(); i++)
	{
		if (path.substr(i, 1) == "/")
		{
			a = b;
		}
		b++;
	}
	Print::Debug("OBJ文件位置：" + path.substr(0, a + 1));
	return path.substr(0, a + 1);

}

vector<TriangleMesh*> loadObj(const std::string filename, TriangleMesh& mesh)
{
	vector<TriangleMesh*> meshs;
	meshs.push_back(&mesh);
	loadObjX(filename, meshs);
	return meshs;
}

void loadObjX(const std::string filename, vector<TriangleMesh*>& _meshs)
{
	using namespace std;
	TriangleMesh* _mesh;
	if (_meshs.size() <= 0)
	{
		_mesh = new TriangleMesh;
		_meshs.push_back(_mesh);
	}
	_mesh = _meshs[0];
	_mesh->path = Get_Xpath(filename);
	std::ifstream in(filename.c_str());

	if (!in.good())
	{
		Print::Debug("ERROR: loading obj:(" + filename + ") file is not good");
		exit(0);
	}

	char buffer[256], str[255];
	float f1, f2, f3;
	int _i_mesh = 0;//模型组计数
	bool read_f = false;//判断是否读取到f前缀
	while (!in.getline(buffer, 255).eof())
	{
		buffer[255] = '\0';

		sscanf_s(buffer, "%s", str, 255);
		std::string s = buffer;

		// reading a vertex //顶点坐标
		if (buffer[0] == 'v' && (buffer[1] == ' ' || buffer[1] == 32))
		{
			if (sscanf(buffer, "v %f %f %f", &f1, &f2, &f3) == 3)
			{
				_mesh->verts.push_back(float3(f1, f2, f3));
			}
			else
			{
				Print::Debug("ERROR: vertex not in wanted format in OBJLoader");
				exit(-1);
			}
		}
		// reading FaceMtls //索引
		else if (buffer[0] == 'f' && (buffer[1] == ' ' || buffer[1] == 32))
		{
			//TriangleFace* ff = new TriangleFace;
			TriangleFace NihaoWanjia;
			if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &NihaoWanjia.v[0], &NihaoWanjia.t[0], &NihaoWanjia.n[0], &NihaoWanjia.v[1], &NihaoWanjia.t[1], &NihaoWanjia.n[1], &NihaoWanjia.v[2], &NihaoWanjia.t[2], &NihaoWanjia.n[2]) == 9)
			{
				_mesh->faces.push_back(NihaoWanjia);
				_mesh->faces_type = 3;
			}
			else if (sscanf(buffer, "f %d//%d %d//%d %d//%d", &NihaoWanjia.v[0], &NihaoWanjia.n[0], &NihaoWanjia.v[1], &NihaoWanjia.n[1], &NihaoWanjia.v[2], &NihaoWanjia.n[2]) == 6)
			{
				_mesh->faces.push_back(NihaoWanjia);
				_mesh->faces_type = 2;
			}
			else if (sscanf(buffer, "f %d/%d %d/%d %d/%d", &NihaoWanjia.v[0], &NihaoWanjia.t[0], &NihaoWanjia.v[1], &NihaoWanjia.t[1], &NihaoWanjia.v[2], &NihaoWanjia.t[2]) == 6)
			{
				_mesh->faces.push_back(NihaoWanjia);
				_mesh->faces_type = 2;
			}
			else if (sscanf(buffer, "f %d %d %d", &NihaoWanjia.v[0], &NihaoWanjia.v[1], &NihaoWanjia.v[2]) == 3)
			{
				_mesh->faces.push_back(NihaoWanjia);
				_mesh->faces_type = 1;
			}
			else
			{
				Print::Debug("ERROR: I don't know the format of that FaceMtl");
				exit(-1);
			}
			read_f = true;
		}
		//法向量
		else if (buffer[0] == 'v' && buffer[1] == 'n' && (buffer[2] == ' ' || buffer[2] == 32))
		{
			vec3 vn;
			if (sscanf(buffer, "vn %f %f %f", &f1, &f2, &f3) == 3)
			{
				vn = vec3(f1, f2, f3);
				_mesh->normals.push_back(vn);
			}
			else
			{
				Print::Debug("错误:读取法向量时获取数据失败");
				exit(-1);
			}
		}
		//纹理坐标
		else if (buffer[0] == 'v' && buffer[1] == 't' && (buffer[2] == ' ' || buffer[2] == 32))
		{
			vec2 vt;
			if (sscanf(buffer, "vt %f %f %f", &f1, &f2, &f3) == 3)
			{
				vt = vec2(f1, f2);
				_mesh->textures.push_back(vt);
			}
			else if (sscanf(buffer, "vt %f %f", &f1, &f2) == 2)
			{
				vt = vec2(f1, f2);
				_mesh->textures.push_back(vt);
			}
			else
			{
				Print::Debug("错误:读取纹理坐标时获取数据失败");
			}
		}
		//渲染属性
		else if (s.substr(0, 6) == "mtllib")
		{
			_mesh->material_mtl = s.substr(7);
		}
		//渲染索引类
		else if (s.substr(0, 6) == "usemtl")
		{
			_mesh->material_usemtl = s.substr(7);
		}
		else if (buffer[0] == 'o' || buffer[0] == 'g' || (read_f == true && buffer[0] == '#') || (read_f == true && s == ""))
		{
			if ((buffer[0] == 'o' && read_f == false) || (buffer[0] == 'g' && read_f == false))
			{
				if (s.substr(2) == "Box_Collider")
				{
					_mesh->active_collider = true;
				}
				else
				{
					_mesh->active_collider = false;
				}
			}
			else
			{
				read_f = false;
				Print::Debug("Obj文件单一模块加载完成:" + to_string(_i_mesh) + "顶点数:" + to_string(_mesh->verts.size()));
				_i_mesh++;
				if (_i_mesh < _meshs.size())
				{
					_mesh = _meshs[_i_mesh];
				}
				else
				{
					TriangleMesh* m = new TriangleMesh;
					_meshs.push_back(m);
					_mesh = _meshs[_i_mesh];
				}
			}
		}
	}
	in.clear();
	in.close();
	if (_meshs[_i_mesh]->verts.size() <= 0)
	{
		delete _meshs[_i_mesh];
		_meshs.pop_back();
		_i_mesh = 0;
	}
	_mesh = _meshs[0];
	Print::Debug("Obj文件单一模块加载完成:" + to_string(_i_mesh) + "顶点数:" + to_string(_mesh->verts.size()));

	float xmin, ymin, zmin, xmax, ymax, zmax;
	int Pxmin, Pxmax, Pymin, Pymax, Pzmin, Pzmax;

	//calculate the bounding sphere
	xmin = xmax = _mesh->verts[0].x;
	ymin = ymax = _mesh->verts[0].y;
	zmin = zmax = _mesh->verts[0].z;
	Pxmin = Pxmax = Pymin = Pymax = Pzmin = Pzmax = 0;

	//calculate the bounding box
	_mesh->bounding_box[0] = float3(_mesh->verts[0].x, _mesh->verts[0].y, _mesh->verts[0].z);
	_mesh->bounding_box[1] = float3(_mesh->verts[0].x, _mesh->verts[0].y, _mesh->verts[0].z);

	for (unsigned int i = 1; i < _mesh->verts.size(); i++)
	{
		//update min value
		_mesh->bounding_box[0].x = std::min(_mesh->verts[i].x, _mesh->bounding_box[0].x);
		_mesh->bounding_box[0].y = std::min(_mesh->verts[i].y, _mesh->bounding_box[0].y);
		_mesh->bounding_box[0].z = std::min(_mesh->verts[i].z, _mesh->bounding_box[0].z);

		//update max value
		_mesh->bounding_box[1].x = std::max(_mesh->verts[i].x, _mesh->bounding_box[1].x);
		_mesh->bounding_box[1].y = std::max(_mesh->verts[i].y, _mesh->bounding_box[1].y);
		_mesh->bounding_box[1].z = std::max(_mesh->verts[i].z, _mesh->bounding_box[1].z);

		//update the  x min and max
		if (_mesh->verts[i].x < xmin) {
			xmin = _mesh->verts[i].x;
			Pxmin = i;
		}
		else if (_mesh->verts[i].x > xmax) {
			xmax = _mesh->verts[i].x;
			Pxmax = i;
		}
		//update the y min and max
		if (_mesh->verts[i].y < ymin) {
			ymin = _mesh->verts[i].y;
			Pymin = i;
		}
		else if (_mesh->verts[i].y > ymax) {
			ymax = _mesh->verts[i].y;
			Pymax = i;
		}
		//update the z min and max
		if (_mesh->verts[i].z < zmin) {
			zmin = _mesh->verts[i].z;
			Pzmin = i;
		}
		else if (_mesh->verts[i].z > zmax) {
			zmax = _mesh->verts[i].z;
			Pzmax = i;
		}
	}

	//calculate the bounding sphere
	float3 dVx = _mesh->verts[Pxmax] - _mesh->verts[Pxmin];
	float3 dVy = _mesh->verts[Pymax] - _mesh->verts[Pymin];
	float3 dVz = _mesh->verts[Pzmax] - _mesh->verts[Pzmin];
	float dx2 = norm2(dVx);
	float dy2 = norm2(dVy);
	float dz2 = norm2(dVz);

	float3 center;
	float  radius2;
	float  radius;

	if (dx2 >= dy2 && dx2 >= dz2) {					// x direction is largest extent  
		center = _mesh->verts[Pxmin] + (dVx / 2.0);	// Center = midpoint of extremes  
		radius2 = norm2(_mesh->verts[Pxmax] - center);// radius squared  
	}
	else if (dy2 >= dx2 && dy2 >= dz2) {				// y direction is largest extent  
		center = _mesh->verts[Pymin] + (dVy / 2.0);	// Center = midpoint of extremes  
		radius2 = norm2(_mesh->verts[Pymax] - center);// radius squared  
	}
	else {
		center = _mesh->verts[Pzmin] + (dVz / 2.0);	// Center = midpoint of extremes  
		radius2 = norm2(_mesh->verts[Pzmax] - center);// radius squared  	
	}

	radius = sqrt(radius2);

	// now check that all points V[i] are in the ball  
	// and if not, expand the ball just enough to include them  
	float3 dV;
	float dist2, dist;
	for (unsigned int i = 0; i < _mesh->verts.size(); i++)
	{
		dV = _mesh->verts[i] - center;
		dist2 = norm2(dV);
		if (dist2 <= radius2) // V[i] is inside the ball already  
			continue;

		// V[i] not in ball, so expand ball to include it  
		dist = sqrt(dist2);
		radius = (radius + dist) / 2.0;         // enlarge radius just enough  
		radius2 = radius * radius;
		center = center + ((dist - radius) / dist) * dV;   // shift Center toward V[i]  

	}

	_mesh->bounding_sphere_c = center;
	_mesh->bounding_sphere_r = radius;

	std::cout << "----------obj file loaded-------------" << endl;
	std::cout << "number of faces:" << _mesh->faces.size() << " number of vertices:" << _mesh->verts.size() << endl;
	std::cout << "obj bounding box: min:("
		<< _mesh->bounding_box[0].x << "," << _mesh->bounding_box[0].y << "," << _mesh->bounding_box[0].z << ") max:("
		<< _mesh->bounding_box[1].x << "," << _mesh->bounding_box[1].y << "," << _mesh->bounding_box[1].z << ")" << endl
		<< "obj bounding sphere center:" << _mesh->bounding_sphere_c.x << "," << _mesh->bounding_sphere_c.y << "," << _mesh->bounding_sphere_c.z << endl
		<< "obj bounding sphere radius:" << _mesh->bounding_sphere_r << endl;

	Print::Debug("Obj文件加载完成:" + to_string(_mesh->verts.size()));

}

vector<TriangleMesh*> loadMtl(const std::string filename, TriangleMesh& _mesh)
{
	vector<TriangleMesh*> meshs;
	meshs.push_back(&_mesh);
	loadMtlX(filename, meshs);
	return meshs;
}

void loadMtlX(const std::string filename, vector<TriangleMesh*>& _meshs)
{
	TriangleMesh* _mesh;
		_mesh = _meshs[0];
		std::ifstream in(filename.c_str());
		if (!in.good())
		{
			std::cout << "ERROR: loading objMtl:(" << filename << ") file is not good" << "\n";
			return;
			exit(0);
		}
		char buffer[256], str[255];
		float f1, f2, f3;
		bool first = true;
		int _i_mesh = 0;
		while (!in.getline(buffer, 255).eof())
		{
			buffer[255] = '\0';
			sscanf_s(buffer, "%s", str, 255);
			std::string s = buffer;


			//漫反射贴图
			if (s.substr(0, 6) == "map_Kd" && (buffer[6] == ' ' || buffer[6] == 32))
			{
				_mesh->map_di = s.substr(7);
			}
			else if (s.substr(0, 6 + 1) == "\tmap_Kd")
			{
				_mesh->map_di = s.substr(7 + 1);
			}
			//镜面反射贴图
			else if (s.substr(0, 6) == "map_Ks")
			{
				_mesh->map_sp = s.substr(7);
			}
			else if (s.substr(0, 6 + 1) == "\tmap_Ks")
			{
				_mesh->map_sp = s.substr(7 + 1);
			}
			//法线贴图(凹凸纹理)
			else if (s.substr(0, 8) == "map_Bump")
			{
				_mesh->map_no = s.substr(9);
			}
			else if (s.substr(0, 8 + 1) == "\tmap_Bump")
			{
				_mesh->map_sp = s.substr(9 + 1);
			}
			//反射贴图
			else if (s.substr(0, 6) == "map_Ka")
			{
				_mesh->map_re = s.substr(7);
			}
			else if (s.substr(0, 6 + 1) == "\tmap_Ka")
			{
				_mesh->map_re = s.substr(7 + 1);
			}
			else if (s.substr(0, 6) == "newmtl")
			{
				if (_i_mesh < _meshs.size())
				{
					//获取到渲染类的名字
					string name = s.substr(7);
					int _count = -1;
					//遍历出正确的类对应的网格对象
					for (int i = 0; i < _meshs.size(); i++)
					{
						if (_meshs[i]->material_usemtl == name)
						{
							_count = i;
							_i_mesh = i;
							if(first)Print::Debug("读取Mtl文件小项:" + _mesh->map_di + _mesh->map_sp + _mesh->map_no);
							_mesh = _meshs[i];
							break;
						}
					}
					//如果未遍历出正确的对象
					if (_count == -1)
					{
						if (!first)_i_mesh++;
						_mesh = _meshs[_i_mesh];
						//_mesh->material_usemtl = s;
					}
				}
				else
				{
					_i_mesh--;
					break;
				}
				first = false;
			}
		}
		in.clear();
		in.close();
		if (_meshs[_i_mesh]->verts.size() <= 0)
		{
			delete _meshs[_i_mesh];
			_meshs.pop_back();
			_i_mesh = 0;
		}
		//_mesh = _meshs[0];
		Print::Debug("读取Mtl文件:" + _mesh->map_di + _mesh->map_sp + _mesh->map_no);
}


class OBJMesh
{
private:
	struct oBJTexture
	{
		string mtl_name;
		GLuint texture1;
		GLuint texture2;
		GLuint texture3;
		GLuint texture4;
	};
	struct MeshVertices
	{
		vector<float3> verts;
		vector<vec3> normals;
		vector<vec2> textures;
	};
private:
	bool first;
	bool active_collider = false;
	Shader shader;
	string path;
private:
	TriangleMesh mesh_true;
	vector<TriangleMesh*> meshs;
	vector<float*> meshs_vertice;
	vector<int*> meshs_verticeSize;
	vector<GLuint*> meshs_vao;
	vector<oBJTexture*> meshs_texture;
	vector<Collider_Coboid*> meshs_collider;
public:
	OBJMesh();

	OBJMesh(string _path)
	{
		loadObjX(_path, meshs);
		loadMtlX(meshs[0]->path + meshs[0]->material_mtl, meshs);
		path = meshs[0]->path;
	}

	OBJMesh(string _path, bool _active_collider)
	{
		active_collider = _active_collider;
		loadObjX(_path, meshs);
		loadMtlX(meshs[0]->path + meshs[0]->material_mtl, meshs);
		path = meshs[0]->path;
	}

	~OBJMesh()
	{
		Print::Debug("正在尝试删除OBJ对象...");
		try
		{
			for (int i = 0; i < meshs.size(); i++)
			{
				delete meshs[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			for (int i = 0; i < meshs_vertice.size(); i++)
			{
				delete[] meshs_vertice[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			for (int i = 0; i < meshs_verticeSize.size(); i++)
			{
				delete meshs_verticeSize[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			for (int i = 0; i < meshs_vao.size(); i++)
			{
				delete meshs_vao[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			bool _mtl_chongfu = false;
			vector<string>* _mtl_name = new vector<string>;
			for (int i = 0; i < meshs_texture.size(); i++)
			{
				for (int mi = 0; mi < _mtl_name->size(); mi++)
				{
					if ((*_mtl_name)[mi] == meshs_texture[i]->mtl_name)
						_mtl_chongfu = true;
				}
				if (!_mtl_chongfu)
				{
					_mtl_name->push_back(meshs_texture[i]->mtl_name);
					delete meshs_texture[i];
				}
				else
				{
					_mtl_chongfu = false;
				}
			}
			delete _mtl_name;
		}
		catch (exception e)
		{
			string s = e.what();
			Print::Debug("尝试删除meshs_texture对象时出现问题:" + s);
		}
		try
		{
			if (meshs_collider.size() > 0)
			{
				for (int i = 0; i < meshs_collider.size(); i++)
				{
					delete meshs_collider[i];
				}
			}
		}
		catch (exception e)
		{
			string s = e.what();
			Print::Debug("尝试删除meshs_collider对象时出现问题:" + s);
		}
		Print::Debug("删除OBJ对象成功.");
	}
public:
	////绘制网格模型(位置)
	//void Draw(vec3 position);
	////顶点数组转化为string类型(顶点数组地址，数组长度)
	//string vertTostring(float* vert, int count);
	////加载OBJ顶点
	//float* GetOBJVertices(TriangleMesh _mesh, int* return_vertices_size);
	////加载OBJ文件对象(顶点数组，长度)
	//GLuint LoadOBJVAO(float* _vertices, int _vertices_size);
	////加载OBJ文件纹理贴图
	//void LoadOBJTexture(TriangleMesh _mesh, GLuint* texture1, GLuint* texture2, GLuint* texture3, GLuint* texture4);
	////加载所有网格的数据
	//void LoadMeshs();
public:
	void First()
	{
		if (first)
		{
			shader = Shader(Path_Shader + "multiple.vert", Path_Shader + "multiple.frag");
			//shader = Shader(Path_Shader + "cube.vert", Path_Shader + "cube.frag");
			Load_shader(shader);
			LoadMeshs();
			first = false;
		}
	}
	//绘制网格模型(位置)
	void Draw(vec3 position)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			if (true)
			{
				//AddCollider(position);
				Load_shader_u(shader, true);
				//shader.setBool("material.active_normalMap", true);
				Load_matrix_model(&Model_Mat4, position);
				//Print::Debug("当前纹理对象参数：" + to_string(meshs_texture[i]->texture1));
				//if (meshs_texture[i]->texture1 == 0)meshs_texture[i]->texture1 = 1;
				Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
			}
			else if (!meshs[i]->active_collider)
			{
				Load_shader_u(shader, true);
				Load_matrix_model(&Model_Mat4, position);
				Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
			}
			else
			{
				//AddCollider(position);
			}
		}
	}
	void Draw(vec3 _position, vec3 _scale)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			if (true)
			{
				//AddCollider(_position);
				Load_shader_u(shader, true);
				//shader.setBool("material.active_normalMap", true);
				Load_matrix_model(&Model_Mat4, _position, _scale);
				//Print::Debug(to_string(i) + "当前纹理对象参数：" + to_string(meshs_texture[i]->texture1));
				//if (meshs_texture[i]->texture1 == 0)meshs_texture[i]->texture1 = 1;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, meshs_texture[i]->texture1);
				Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
			}
			else if (!meshs[i]->active_collider)
			{
				Load_shader_u(shader, true);
				//shader.setBool("material.active_normalMap", true);
				Load_matrix_model(&Model_Mat4, _position, _scale);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, meshs_texture[i]->texture1);
				Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
			}
			else
			{
				//AddCollider(_position);
			}
		}
	}
	void Draw_Collider(vec3 _position)
	{
		if (active_collider)
		{
			AddCollider(_position);
		}
	}
	void Draw_Collider(vec3 _position, vec3 _scale)
	{
		if (active_collider)
		{
			AddCollider(_position);
		}
	}
	void Draw_Shadow(vec3 _position)
	{
		First();
		Load_shader_u(shader, true);
		for (int i = 0; i < meshs.size(); i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			Load_matrix_model(&_model, _position);
			Open_Shadow_SetShaderMat4("model", _model);
			glBindVertexArray(*meshs_vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, *(meshs_verticeSize[i]) / 8);
		}
	}
	void Draw_SSAO(Shader& _shader)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			_shader.use();
			glBindVertexArray(*meshs_vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, *(meshs_verticeSize[i]) / 8);
		}
	}
	void Draw_SSAO(vec3 _position, Shader &_shader)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			Load_matrix_model(&_model, _position);
			shaderGeometryPass_SSAO.use();
			shaderGeometryPass_SSAO.setMat4("model", _model);
			glBindVertexArray(*meshs_vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, *(meshs_verticeSize[i]) / 8);
		}
	}
	//绘制多个模型(位置数组，数量)
	void Draws(vec3* _position, int count)
	{
		First();
		for (int ii = 0; ii < count; ii++)
		{
			for (int i = 0; i < meshs.size(); i++)
			{
				if (true)
				{
					//AddCollider(position);
					Load_shader_u(shader, true);
					//shader.setBool("material.active_normalMap", true);
					Load_matrix_model(&Model_Mat4, _position[ii]);
					//Print::Debug("当前纹理对象参数：" + to_string(meshs_texture[i]->texture1));
					//if (meshs_texture[i]->texture1 == 0)meshs_texture[i]->texture1 = 1;
					Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
				}
			}
		}
		//Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
	}
	//加载所有网格的数据
	void LoadMeshs()
	{
		//将顶点对象排序到一个模型中
		for (int i = 0; i < meshs.size(); i++)
		{
			vector<float3>* vf = &meshs[i]->verts;
			for (int vi = 0; vi < vf->size(); vi++)
			{
				mesh_true.verts.push_back((*vf)[vi]);
			}
			vector<vec3>* vn = &meshs[i]->normals;
			for (int vi = 0; vi < vn->size(); vi++)
			{
				mesh_true.normals.push_back((*vn)[vi]);
			}
			vector<vec2>* vt = &meshs[i]->textures;
			for (int vi = 0; vi < vt->size(); vi++)
			{
				mesh_true.textures.push_back((*vt)[vi]);
			}
		}
		Print::Debug("OBJ文件顶点总数：" + to_string(mesh_true.verts.size()));
		//加载相应模块
		{
			vector<oBJTexture*>* vec_mtl = new vector<oBJTexture*>;
			for (int i = 0; i < meshs.size(); i++)
			{
				int* ci = new int(0);
				float* vf = GetOBJVertices(mesh_true, *meshs[i], ci);
				meshs_verticeSize.push_back(ci);
				meshs_vertice.push_back(vf);
				//cout << vertTostring(vf, *meshs_verticeSize[i]) << endl;
				GLuint vao, vbo;
				Load_vertices(&vao, &vbo, vf, *meshs_verticeSize[i] * 4);
				GLuint* vi = new GLuint(vao);//加载相应的vao对象	//LoadOBJVAO(meshs_vertice[i], *meshs_verticeSize[i])
				//Print::Debug("加载相应vao时数组大小：" + to_string(*meshs_verticeSize[i]));
				meshs_vao.push_back(vi);
				//判断是否已有相同的纹理对象
				bool material_ = false;
				for (int veci = 0; veci < vec_mtl->size(); veci++)
				{
					if ((*vec_mtl)[veci]->mtl_name == meshs[i]->material_usemtl)
					{
						material_ = true;
						//找到相同纹理对象时，直接添加地址到当前加载的mesh中
						meshs_texture.push_back((*vec_mtl)[veci]);
					}
				}
				if (!material_)//如果未找到相同的纹理对象
				{
					oBJTexture* ot = new oBJTexture;
					LoadOBJTexture(*meshs[i], &(ot->texture1), &(ot->texture2), &(ot->texture3), &(ot->texture4));
					Print::Debug("纹理对象参数:" + to_string(ot->texture1) + "//" + to_string(ot->texture2) + "//" + to_string(ot->texture3) + "//" + to_string(ot->texture4));
					ot->mtl_name = meshs[i]->material_usemtl;
					//添加已初始化的纹理对象地址到临时创建的数组中；
					vec_mtl->push_back(ot);
					meshs_texture.push_back(ot);
				}
			}
			//删除使用new创建的临时对象
			delete vec_mtl;
			//加载碰撞体对象
			LoadCollider();
		}
		for (int i = 0; i < meshs.size(); i++)
		{
			//Print::Debug("mesh 各面 顶点个数：" + to_string(*meshs_verticeSize[i]));
		}
	}
	//加载OBJ顶点
	float* GetOBJVertices(TriangleMesh _mesh_true, TriangleMesh _mesh, int* return_vertices_size)
	{
		vector<float3>* vertices_f3 = &_mesh_true.verts;
		vector<vec2>* textures_v2 = &_mesh_true.textures;
		vector<vec3>* normals_v3 = &_mesh_true.normals;
		Print::Debug("各vector对象大小：" + to_string(vertices_f3->size()) + "/" + to_string(textures_v2->size()) + "/" + to_string(normals_v3->size()));
		vector<TriangleFace>* element_v3 = &_mesh.faces;
		*return_vertices_size = element_v3->size() * (3 * 8);
		float* vf = new float[*return_vertices_size];
		int vcount = 0;
		//Print::Debug(to_string(element_v3->size()));
		//遍历所有的三角面
		for (int i = 0; i < element_v3->size(); i++)
		{
			//Print::Debug("当前三角面::" + to_string(i));
			//顶点数组索引
			int* vertices_index = (*element_v3)[i].v;
			//Print::Debug("当前三角面顶点数组索引::" + to_string(vertices_index[0]) + "/" + to_string(vertices_index[1]) + "/" + to_string(vertices_index[2]));
			int* textures_index = (*element_v3)[i].t;
			//Print::Debug("当前三角面纹理对象索引::" + to_string(textures_index[0]) + "/" + to_string(textures_index[1]) + "/" + to_string(textures_index[2]));
			int* normals_index = (*element_v3)[i].n;
			//Print::Debug("当前三角面法向量索引::" + to_string(normals_index[0]) + "/" + to_string(normals_index[1]) + "/" + to_string(normals_index[2]));
			int count = 0;
			//写入每个三角面的所有顶点
			for (int ii = 0; ii < (3 + 3 + 3); ii++)
			{
				//Print::Debug("i::" + to_string(i) + "ii::" + to_string(ii));
				switch (ii)
				{
				case 0:
					count = vertices_index[0] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*vertices_f3)[count].x;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].y;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].z;
					}
					break;
				case 3:
					count = vertices_index[1] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*vertices_f3)[count].x;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].y;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].z;
					}
					break;
				case 6:
					count = vertices_index[2] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*vertices_f3)[count].x;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].y;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].z;
					}
					break;
				case 1:
					count = normals_index[0] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*normals_v3)[count].x;
						vcount++;
						vf[vcount] = (*normals_v3)[count].y;
						vcount++;
						vf[vcount] = (*normals_v3)[count].z;
					}
					break;
				case 4:
					count = normals_index[1] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*normals_v3)[count].x;
						vcount++;
						vf[vcount] = (*normals_v3)[count].y;
						vcount++;
						vf[vcount] = (*normals_v3)[count].z;
					}
					break;
				case 7:
					count = normals_index[2] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*normals_v3)[count].x;
						vcount++;
						vf[vcount] = (*normals_v3)[count].y;
						vcount++;
						vf[vcount] = (*normals_v3)[count].z;
					}
					break;
				case 2:
					count = textures_index[0] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*textures_v2)[count].x;
						vcount++;
						vf[vcount] = (*textures_v2)[count].y;
					}
					break;
				case 5:
					count = textures_index[1] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*textures_v2)[count].x;
						vcount++;
						vf[vcount] = (*textures_v2)[count].y;
					}
					break;
				case 8:
					count = textures_index[2] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*textures_v2)[count].x;
						vcount++;
						vf[vcount] = (*textures_v2)[count].y;
					}
				}
				vcount++;
			}
		}
		//Print::Debug("OBJ文件顶点数量：" + to_string(*return_vertices_size));
		//Print::Debug("OBJ文件顶点：");
		//cout << vertTostring(vf, *return_vertices_size) << endl;
		return vf;
	}
	//加载OBJ文件对象(顶点数组，长度)
	GLuint LoadOBJVAO(float* _vertices, int _vertices_size)
	{
		GLuint VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices_size, _vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		return VAO;
	}
	//加载OBJ文件对象
	GLuint LoadOBJVAO(float* _vertices, int _vertices_size, float* _indices, int _indices_size)
	{
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices_size, _vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		return VAO;
	}
	//加载OBJ文件纹理贴图
	void LoadOBJTexture(TriangleMesh _mesh, GLuint* texture1, GLuint* texture2, GLuint* texture3, GLuint* texture4)
	{
		//Load_texture(texture1, texture2, path + _mesh.map_di, path + _mesh.map_sp);//
		Load_texture(texture1, texture2, texture3, texture4, path + _mesh.map_di, path + _mesh.map_sp, Path_Resources + "skybox/", path + _mesh.map_no);//_mesh.map_re
		Print::Debug("当前mtl名字：：" + _mesh.material_usemtl);
		Print::Debug(_mesh.material_usemtl + "//" + _mesh.map_di + "//" + _mesh.map_sp + "//" + _mesh.map_no);
	}
	//检查碰撞体参数
	void LoadCollider()
	{
		if (active_collider)
		{
			for (int i = 0; i < meshs_vertice.size(); i++)
			{
				GLfloat xu = 0, xd = 0, yu = 0, yd = 0, zu = 0, zd = 0;
				xu = meshs_vertice[i][0];
				xd = meshs_vertice[i][0];
				yu = meshs_vertice[i][1];
				yd = meshs_vertice[i][1];
				zu = meshs_vertice[i][2];
				zd = meshs_vertice[i][2];
				for (int si = 0; si < *meshs_verticeSize[i] / 8; si++)
				{
					int sic = si * 8;
					if (meshs_vertice[i][sic] > xu)xu = meshs_vertice[i][sic];
					else if (meshs_vertice[i][sic] < xd)xd = meshs_vertice[i][sic];
					if (meshs_vertice[i][sic + 1] > yu)yu = meshs_vertice[i][sic + 1];
					else if (meshs_vertice[i][sic + 1] < yd)yd = meshs_vertice[i][sic + 1];
					if (meshs_vertice[i][sic + 2] > zu)zu = meshs_vertice[i][sic + 2];
					else if (meshs_vertice[i][sic + 2] < zd)zd = meshs_vertice[i][sic + 2];
				}
				Collider_Coboid* cci = new Collider_Coboid(Math::Center(vec3(xu, yu, zu), vec3(xd, yd, zd)), vec3(Math::Distance(xu, xd), Math::Distance(yu, yd), Math::Distance(zu, zd)));
				Print::Debug("碰撞体参数：");
				Print::Debug(to_string(cci->Point().x) + "|" + to_string(cci->Point().y) + "|" + to_string(cci->Point().z));
				Print::Debug(to_string(cci->LWH().x) + "|" + to_string(cci->LWH().y) + "|" + to_string(cci->LWH().z));
				meshs_collider.push_back(cci);
				//cout << xu << "|" << xd << "|" << yu << "|" << yd << "|" << zu << "|" << zd << endl;
				//string s;
				//cin >> s;
				//if (s == "endl")
				//	break;
			}
			Print::Debug(to_string(meshs_collider.size()));
		}
	}
	void AddCollider(vec3 _position)
	{
		for (int i = 0; i < meshs_collider.size(); i++)
		{
			if (Math::Distance_max(_position + meshs_collider[i]->Point(), PlayerPos) < 10)
			{
				Collider_Coboid* cci = new Collider_Coboid(_position + meshs_collider[i]->Point(), meshs_collider[i]->LWH());
				Collider_Layer_Default.collection_entity.push_back(cci);
			}
		}
	}
	void AddCollider(vec3 _position, vector<Collider_Coboid*>* vcci)
	{
		Print::Debug("碰撞体数量::" + to_string(meshs_collider.size()) + "???" + to_string(meshs_collider[0]->LWH().r) + "," + to_string(meshs_collider[0]->LWH().g) + "," + to_string(meshs_collider[0]->LWH().b));
		for (int i = 0; i < meshs_collider.size(); i++)
		{
			//if (Math::Distance_max(_position + meshs_collider[i]->Point(), PlayerPos) < 10)
			{
				Collider_Coboid* cci = new Collider_Coboid(_position + meshs_collider[i]->Point(), meshs_collider[i]->LWH());
				vcci->push_back(cci);
			}
		}
	}
	//顶点数组转化为string类型(顶点数组地址，数组长度)
	string vertTostring(float* vert, int count)
	{
		string s;
		int scount = 0;
		for (int i = 0; i < count / 8; i++)
		{
			for (int ii = 0; ii < 8; ii++)
			{
				s.append(to_string(vert[scount]));
				s.append(",");
				scount++;
			}
			s.append("\r\n");
		}
		return s;
	}
};

class OBJMeshs
{
private:
	struct oBJTexture
	{
		string mtl_name;
		GLuint texture1;
		GLuint texture2;
		GLuint texture3;
		GLuint texture4;
	};
	struct MeshVertices
	{
		vector<float3> verts;
		vector<vec3> normals;
		vector<vec2> textures;
	};
private:
	bool first;
	Shader shader;
	string path;
private:
	TriangleMesh mesh_true;
	vector<TriangleMesh*> meshs;
	vector<float*> meshs_vertice;
	vector<int*> meshs_verticeSize;
	vector<GLuint*> meshs_vao;
	vector<oBJTexture*> meshs_texture;
public:
	OBJMeshs();

	OBJMeshs(string _path)
	{
		loadObjX(_path, meshs);
		loadMtlX(meshs[0]->path + meshs[0]->material_mtl, meshs);
		path = meshs[0]->path;
	}

	~OBJMeshs()
	{
		Print::Debug("正在尝试删除OBJ对象...");
		try
		{
			for (int i = 0; i < meshs.size(); i++)
			{
				delete meshs[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			for (int i = 0; i < meshs_vertice.size(); i++)
			{
				delete[] meshs_vertice[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			for (int i = 0; i < meshs_verticeSize.size(); i++)
			{
				delete meshs_verticeSize[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			for (int i = 0; i < meshs_vao.size(); i++)
			{
				delete meshs_vao[i];
			}
		}
		catch (exception e)
		{

		}
		try
		{
			bool _mtl_chongfu = false;
			vector<string>* _mtl_name = new vector<string>;
			for (int i = 0; i < meshs_texture.size(); i++)
			{
				for (int mi = 0; mi < _mtl_name->size(); mi++)
				{
					if ((*_mtl_name)[mi] == meshs_texture[i]->mtl_name)
						_mtl_chongfu = true;
				}
				if (!_mtl_chongfu)
				{
					_mtl_name->push_back(meshs_texture[i]->mtl_name);
					delete meshs_texture[i];
				}
				else
				{
					_mtl_chongfu = false;
				}
			}
			delete _mtl_name;
		}
		catch (exception e)
		{
			string s = e.what();
			Print::Debug("尝试删除meshs_texture对象时出现问题:" + s);
		}
		Print::Debug("删除OBJ对象成功.");
	}
public:
	////绘制网格模型(位置)
	//void Draw(vec3 position);
	////顶点数组转化为string类型(顶点数组地址，数组长度)
	//string vertTostring(float* vert, int count);
	////加载OBJ顶点
	//float* GetOBJVertices(TriangleMesh _mesh, int* return_vertices_size);
	////加载OBJ文件对象(顶点数组，长度)
	//GLuint LoadOBJVAO(float* _vertices, int _vertices_size);
	////加载OBJ文件纹理贴图
	//void LoadOBJTexture(TriangleMesh _mesh, GLuint* texture1, GLuint* texture2, GLuint* texture3, GLuint* texture4);
	////加载所有网格的数据
	//void LoadMeshs();
public:
	void First()
	{
		if (first)
		{
			shader = Shader(Path_Shader + "multiple.vert", Path_Shader + "multiple.frag");
			//shader = Shader(Path_Shader + "cube.vert", Path_Shader + "cube.frag");
			Load_shader(shader);
			LoadMeshs();
			first = false;
		}
	}
	//绘制网格模型(位置)
	void Draw(vec3 position)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			Load_shader_u(shader, true);
			//shader.setBool("material.active_normalMap", true);
			Load_matrix_model(&Model_Mat4, position);
			//Print::Debug("当前纹理对象参数：" + to_string(meshs_texture[i]->texture1));
			//if (meshs_texture[i]->texture1 == 0)meshs_texture[i]->texture1 = 1;
			Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
		}
	}
	void Draw(vec3 _position, vec3 _scale)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			Load_shader_u(shader, true);
			//shader.setBool("material.active_normalMap", true);
			Load_matrix_model(&Model_Mat4, _position, _scale);
			//Print::Debug(to_string(i) + "当前纹理对象参数：" + to_string(meshs_texture[i]->texture1));
			//if (meshs_texture[i]->texture1 == 0)meshs_texture[i]->texture1 = 1;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, meshs_texture[i]->texture1);
			Load_draw(meshs_vao[i], &(meshs_texture[i]->texture1), &(meshs_texture[i]->texture2), &(meshs_texture[i]->texture3), &(meshs_texture[i]->texture4), shader, *(meshs_verticeSize[i]) / 8);
		}
	}
	void Draw_Shadow(vec3 _position)
	{
		First();
		for (int i = 0; i < meshs.size(); i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			Load_matrix_model(&_model, _position);
			Open_Shadow_SetShaderMat4("model", _model);
			glBindVertexArray(*meshs_vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, *(meshs_verticeSize[i]) / 8);
		}
	}
	//绘制多个模型(位置数组，数量)
	void Draws(vec3* _position, int count)
	{
		First();
		shader.use();
		shader.setMat4("projection", Projection_Mat4);
		shader.setMat4("view", View_Mat4);
		for (int i = 0; i < count; i++)
		{
			Load_matrix_model(&Model_Mat4, _position[i]);
			shader.setMat4("model", Model_Mat4);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
	}
	//加载所有网格的数据
	void LoadMeshs()
	{
		//将顶点对象排序到一个模型中
		for (int i = 0; i < meshs.size(); i++)
		{
			vector<float3>* vf = &meshs[i]->verts;
			for (int vi = 0; vi < vf->size(); vi++)
			{
				mesh_true.verts.push_back((*vf)[vi]);
			}
			vector<vec3>* vn = &meshs[i]->normals;
			for (int vi = 0; vi < vn->size(); vi++)
			{
				mesh_true.normals.push_back((*vn)[vi]);
			}
			vector<vec2>* vt = &meshs[i]->textures;
			for (int vi = 0; vi < vt->size(); vi++)
			{
				mesh_true.textures.push_back((*vt)[vi]);
			}
		}
		Print::Debug("OBJ文件顶点总数：" + to_string(mesh_true.verts.size()));
		//加载相应模块
		{
			vector<oBJTexture*>* vec_mtl = new vector<oBJTexture*>;
			for (int i = 0; i < meshs.size(); i++)
			{
				int* ci = new int(0);
				float* vf = GetOBJVertices(mesh_true, *meshs[i], ci);
				meshs_verticeSize.push_back(ci);
				meshs_vertice.push_back(vf);
				//cout << vertTostring(vf, *meshs_verticeSize[i]) << endl;
				GLuint vao, vbo;
				Load_vertices(&vao, &vbo, vf, *meshs_verticeSize[i] * 4);
				GLuint* vi = new GLuint(vao);//加载相应的vao对象	//LoadOBJVAO(meshs_vertice[i], *meshs_verticeSize[i])
				//Print::Debug("加载相应vao时数组大小：" + to_string(*meshs_verticeSize[i]));
				meshs_vao.push_back(vi);
				//判断是否已有相同的纹理对象
				bool material_ = false;
				for (int veci = 0; veci < vec_mtl->size(); veci++)
				{
					if ((*vec_mtl)[veci]->mtl_name == meshs[i]->material_usemtl)
					{
						material_ = true;
						//找到相同纹理对象时，直接添加地址到当前加载的mesh中
						meshs_texture.push_back((*vec_mtl)[veci]);
					}
				}
				if (!material_)//如果未找到相同的纹理对象
				{
					oBJTexture* ot = new oBJTexture;
					LoadOBJTexture(*meshs[i], &(ot->texture1), &(ot->texture2), &(ot->texture3), &(ot->texture4));
					Print::Debug("纹理对象参数:" + to_string(ot->texture1) + "//" + to_string(ot->texture2) + "//" + to_string(ot->texture3) + "//" + to_string(ot->texture4));
					ot->mtl_name = meshs[i]->material_usemtl;
					//添加已初始化的纹理对象地址到临时创建的数组中；
					vec_mtl->push_back(ot);
					meshs_texture.push_back(ot);
				}
			}
			//删除使用new创建的临时对象
			delete vec_mtl;
		}
		for (int i = 0; i < meshs.size(); i++)
		{
			//Print::Debug("mesh 各面 顶点个数：" + to_string(*meshs_verticeSize[i]));
		}
	}
	//加载OBJ顶点
	float* GetOBJVertices(TriangleMesh _mesh_true, TriangleMesh _mesh, int* return_vertices_size)
	{
		vector<float3>* vertices_f3 = &_mesh_true.verts;
		vector<vec2>* textures_v2 = &_mesh_true.textures;
		vector<vec3>* normals_v3 = &_mesh_true.normals;
		Print::Debug("各vector对象大小：" + to_string(vertices_f3->size()) + "/" + to_string(textures_v2->size()) + "/" + to_string(normals_v3->size()));
		vector<TriangleFace>* element_v3 = &_mesh.faces;
		*return_vertices_size = element_v3->size() * (3 * 8);
		float* vf = new float[*return_vertices_size];
		int vcount = 0;
		//Print::Debug(to_string(element_v3->size()));
		//遍历所有的三角面
		for (int i = 0; i < element_v3->size(); i++)
		{
			//Print::Debug("当前三角面::" + to_string(i));
			//顶点数组索引
			int* vertices_index = (*element_v3)[i].v;
			//Print::Debug("当前三角面顶点数组索引::" + to_string(vertices_index[0]) + "/" + to_string(vertices_index[1]) + "/" + to_string(vertices_index[2]));
			int* textures_index = (*element_v3)[i].t;
			//Print::Debug("当前三角面纹理对象索引::" + to_string(textures_index[0]) + "/" + to_string(textures_index[1]) + "/" + to_string(textures_index[2]));
			int* normals_index = (*element_v3)[i].n;
			//Print::Debug("当前三角面法向量索引::" + to_string(normals_index[0]) + "/" + to_string(normals_index[1]) + "/" + to_string(normals_index[2]));
			int count = 0;
			//写入每个三角面的所有顶点
			for (int ii = 0; ii < (3 + 3 + 3); ii++)
			{
				//Print::Debug("i::" + to_string(i) + "ii::" + to_string(ii));
				switch (ii)
				{
				case 0:
					count = vertices_index[0] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*vertices_f3)[count].x;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].y;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].z;
					}
					break;
				case 3:
					count = vertices_index[1] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*vertices_f3)[count].x;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].y;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].z;
					}
					break;
				case 6:
					count = vertices_index[2] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*vertices_f3)[count].x;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].y;
						vcount++;
						vf[vcount] = (*vertices_f3)[count].z;
					}
					break;
				case 1:
					count = normals_index[0] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*normals_v3)[count].x;
						vcount++;
						vf[vcount] = (*normals_v3)[count].y;
						vcount++;
						vf[vcount] = (*normals_v3)[count].z;
					}
					break;
				case 4:
					count = normals_index[1] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*normals_v3)[count].x;
						vcount++;
						vf[vcount] = (*normals_v3)[count].y;
						vcount++;
						vf[vcount] = (*normals_v3)[count].z;
					}
					break;
				case 7:
					count = normals_index[2] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*normals_v3)[count].x;
						vcount++;
						vf[vcount] = (*normals_v3)[count].y;
						vcount++;
						vf[vcount] = (*normals_v3)[count].z;
					}
					break;
				case 2:
					count = textures_index[0] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*textures_v2)[count].x;
						vcount++;
						vf[vcount] = (*textures_v2)[count].y;
					}
					break;
				case 5:
					count = textures_index[1] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*textures_v2)[count].x;
						vcount++;
						vf[vcount] = (*textures_v2)[count].y;
					}
					break;
				case 8:
					count = textures_index[2] - 1;
					if (count < 0)
					{
						vf[vcount] = 0;
						vcount++;
						vf[vcount] = 0;
					}
					else
					{
						vf[vcount] = (*textures_v2)[count].x;
						vcount++;
						vf[vcount] = (*textures_v2)[count].y;
					}
				}
				vcount++;
			}
		}
		//Print::Debug("OBJ文件顶点数量：" + to_string(*return_vertices_size));
		//Print::Debug("OBJ文件顶点：");
		//cout << vertTostring(vf, *return_vertices_size) << endl;
		return vf;
	}
	//加载OBJ文件对象(顶点数组，长度)
	GLuint LoadOBJVAO(float* _vertices, int _vertices_size)
	{
		GLuint VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices_size, _vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		return VAO;
	}
	//加载OBJ文件对象
	GLuint LoadOBJVAO(float* _vertices, int _vertices_size, float* _indices, int _indices_size)
	{
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices_size, _vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		return VAO;
	}
	//加载OBJ文件纹理贴图
	void LoadOBJTexture(TriangleMesh _mesh, GLuint* texture1, GLuint* texture2, GLuint* texture3, GLuint* texture4)
	{
		//Load_texture(texture1, texture2, path + _mesh.map_di, path + _mesh.map_sp);//
		Load_texture(texture1, texture2, texture3, texture4, path + _mesh.map_di, path + _mesh.map_sp, Path_Resources + "skybox/", path + _mesh.map_no);//_mesh.map_re
		Print::Debug("当前mtl名字：：" + _mesh.material_usemtl);
		Print::Debug(_mesh.material_usemtl + "//" + _mesh.map_di + "//" + _mesh.map_sp + "//" + _mesh.map_no);
	}
	void LoadCollider(TriangleMesh _mesh)
	{
		if (_mesh.active_collider)
		{

		}
	}
	//顶点数组转化为string类型(顶点数组地址，数组长度)
	string vertTostring(float* vert, int count)
	{
		string s;
		int scount = 0;
		for (int i = 0; i < count / 8; i++)
		{
			for (int ii = 0; ii < 8; ii++)
			{
				s.append(to_string(vert[scount]));
				s.append(",");
				scount++;
			}
			s.append("\r\n");
		}
		return s;
	}
};

#endif

