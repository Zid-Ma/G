#pragma once
class Cube
{
private:
	static bool first;
protected:
	static GLuint vao, vbo;
	static Shader shader;

public:

	Cube()
	{
		First();
	}

	static void First()
	{
		//判断是否为第一次加载该模型
		if (first)
		{
			Load_vertices(&vao, &vbo, Vertices_cube, sizeof(Vertices_cube));
			shader = Shader::Shader(Path_Shader + "multiple.vert", Path_Shader + "multiple.frag");
			Load_shader(shader);
			first = false;
		}
	}

	static GLuint GetVao()
	{
		return vao;
	}
};
bool Cube::first = true;
GLuint Cube::vao = 0, Cube::vbo = 0;
Shader Cube::shader;

class XYKSBox :Cube
{
private:
	static bool first;
	static GLuint texture0, texture1, texture2;
public:

	XYKSBox()
	{
		First();
	}

	XYKSBox(vec3 position)
	{
		Draw(position);
	}

	static void First()
	{
		if (first)
		{
			Cube cube;
			Load_texture(&texture0, &texture1, Path_Resources + "Map/xyks.bmp", Path_Resources + "Map/xyks.bmp");
			first = false;
		}
	}

	static void Draw(vec3 _position)
	{
		First();
		Load_shader_u(Cube::shader, true);
		Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
		Load_draw(&Cube::vao, &texture0, &texture1, Cube::shader);
	}

	static void Draw_Shadow(vec3 _position)
	{
		First();
		glm::mat4 _model = glm::mat4(1.0f);
		Load_matrix_model(&_model, _position);
		Open_Shadow_SetShaderMat4("model", _model);
		glBindVertexArray(Cube::vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};
bool XYKSBox::first = true;
GLuint XYKSBox::texture0 = 0, XYKSBox::texture1 = 0, XYKSBox::texture2 = 0;

class WoodenBox :Cube
{
private:
	static bool first;
	static GLuint texture0, texture1, texture2;
public:

	WoodenBox()
	{
		First();
	}

	WoodenBox(vec3 position)
	{
		Draw(position);
	}

	static void First()
	{
		if (first)
		{
			Cube cube;
			Load_texture(&texture0, &texture1, &texture2, Path_Resources + "Map/WoodenBox-DiffuseMap.bmp", Path_Resources + "Map/WoodenBox-SpecularMap.bmp", Path_Resources + "skybox/");
			first = false;
		}
	}

	static void Draw(vec3 position)
	{
		First();
		Load_shader_u(Cube::shader, true);
		Load_matrix_model(&Model_Mat4, position, vec3(1.0f));
		Load_draw(&Cube::vao, &texture0, &texture1, Cube::shader);
	}

	static void Draw_Shadow(vec3 _position)
	{
		First();
		glm::mat4 _model = glm::mat4(1.0f);
		Load_matrix_model(&_model, _position);
		Open_Shadow_SetShaderMat4("model", _model);
		glBindVertexArray(Cube::vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};
bool WoodenBox::first = true;
GLuint WoodenBox::texture0 = 0, WoodenBox::texture1 = 0, WoodenBox::texture2 = 0;

