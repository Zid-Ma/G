#pragma once
class Square
{
private:
	static bool first;
protected:
	static GLuint vao, vbo;
	static Shader shader;
	static Shader shader_Font;
public:

	Square()
	{
		First();
	}

	static void First()
	{
		//判断是否为第一次加载该模型
		if (first)
		{
			Load_vertices(&vao, &vbo, Vertices_square, sizeof(Vertices_square));
			shader = Shader::Shader(Path_Shader + "multiple.vert", Path_Shader + "multiple.frag");
			Load_shader(shader);
			shader_Font = Shader::Shader(Path_Shader + "multiple.vert", Path_Shader + "font.frag");
			Load_shader(shader);
			first = false;
		}
	}
	static void Draw(GLuint texture, vec3 _position)
	{
		First();
		Load_shader_u(shader,true);
		Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
		Load_draw(&Square::vao, &texture, Square::shader);
	}
	static void Draw(GLuint texture, vec3 _position, Shader &_shader)
	{
		First();
		//Load_shader_u(_shader, true);
		//ui->Use_fontShader();

		Load_shader_u(shader_Font, true);
		Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
		Load_draw(&Square::vao, &texture, shader_Font);

		//_shader.use();
		//Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
		//_shader.setMat4("projection", Projection_Mat4);
		//_shader.setMat4("view", View_Mat4);
		//_shader.setMat4("model", Model_Mat4);
		//_shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);
		//glBindVertexArray(Square::vao);
		//glActiveTexture(GL_TEXTURE0);
		////绑定纹理单元
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};
bool Square::first = true;
GLuint Square::vao = 0, Square::vbo = 0;
Shader Square::shader;
Shader Square::shader_Font;

class XYKS:Square
{
private:
	static bool first;
	static GLuint texture0, texture1, texture2;
public:

	XYKS()
	{
		First();
	}

	XYKS(vec3 position)
	{
		Draw(position);
	}

	static void First()
	{
		if (first)
		{
			Square square;
			Load_texture(&texture0, &texture1, &texture2, Path_Resources + "Map/xyks.bmp", Path_Resources + "Map/xyks.bmp", Path_Resources + "skybox/");
			first = false;
		}
	}

	static void Draw(vec3 _position)
	{
		First();
		Load_shader_u(Square::shader, true);
		Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
		Load_draw(&Square::vao, &texture0, &texture1, Square::shader);
	}

	static void Draw_Shadow(vec3 _position)
	{
		First();
		glm::mat4 _model = glm::mat4(1.0f);
		Load_matrix_model(&_model, _position);
		Open_Shadow_SetShaderMat4("model", _model);
		glBindVertexArray(Square::vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	static void Draw_SSAO(vec3 _position, Shader& _shader)
	{
		First();
		glm::mat4 _model = glm::mat4(1.0f);
		Load_matrix_model(&_model, _position);
		shaderGeometryPass_SSAO.use();
		shaderGeometryPass_SSAO.setMat4("model", _model);
		glBindVertexArray(Square::vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};
bool XYKS::first = true;
GLuint XYKS::texture0 = 0, XYKS::texture1 = 0, XYKS::texture2 = 0;

class Grass :Square
{
private:
	static bool first;
	static GLuint texture0, texture1, texture2;
public:

	Grass()
	{
		First();
	}

	Grass(vec3 position)
	{
		Draw(position);
	}

	static void First()
	{
		if (first)
		{
			Square square;
			Load_texture(&texture0, &texture1, &texture2, Path_Resources + "Map/grass.png", Path_Resources + "Map/xss.bmp", Path_Resources + "skybox/");
			first = false;
		}
	}

	static void Draw(vec3 _position)
	{
		First();
		Load_shader_u(Square::shader, true);
		Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
		Load_draw(&Square::vao, &texture0, &texture1, Square::shader);
	}

	static void Draw_Shadow(vec3 _position)
	{
		First();
		glm::mat4 _model = glm::mat4(1.0f);
		Load_matrix_model(&_model, _position);
		Open_Shadow_SetShaderMat4("model", _model);
		glBindVertexArray(Square::vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};
bool Grass::first = true;
GLuint Grass::texture0 = 0, Grass::texture1 = 0, Grass::texture2 = 0;
