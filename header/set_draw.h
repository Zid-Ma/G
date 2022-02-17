#pragma once

static void Load_draw(GLuint* VAO, Shader& shader);
//绘制图形(顶点数组对象，漫反射纹理对象，着色器)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, Shader& shader);
//绘制图形(顶点数组对象，漫反射纹理对象，镜面反射纹理对象，着色器)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, Shader& shader);
//绘制图形(顶点数组对象，漫反射纹理对象，镜面反射纹理对象，环境映射纹理对象，着色器)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, Shader& shader);
//绘制图形(顶点数组对象，漫反射纹理对象，镜面反射纹理对象，环境映射纹理对象，法线贴图纹理对象，着色器)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader);
//绘制图形(顶点数组对象，漫反射纹理对象，镜面反射纹理对象，环境映射纹理对象，法线贴图纹理对象，着色器, 顶点数量)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader, int _verticesCout);

static void Load_draw_multiple(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, Shader& shader);

static void Load_draw(GLuint* VAO, Shader& shader)
{
	//绑定VAO对象
	glBindVertexArray(*VAO);

	//获取相应着色器对象并使用
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, Shader& shader)
{
	//绑定VAO对象
	glBindVertexArray(*VAO);

	//获取相应着色器对象并使用
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//在相应的纹理单元上绑定纹理
	//--------------------------
	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理单元
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//天空盒子映射
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, Shader& shader)
{
	//绑定VAO对象
	glBindVertexArray(*VAO);

	//获取相应着色器对象并使用
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//在相应的纹理单元上绑定纹理
	//--------------------------
	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理单元
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//天空盒子映射
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint *VAO, GLuint *Textrure1, GLuint* Textrure2, GLuint* Textrure3, Shader &shader)
{
	//绑定VAO对象
	glBindVertexArray(*VAO);

	//if (FirstMat4) 
	//{
	//	判断是否为第一次绘制，如果是，则将透视矩阵、视图矩阵初始化
	//	Load_mat4();
	//	FirstMat4 = false;
	//}
	//获取相应着色器对象并使用
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model",Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//在相应的纹理单元上绑定纹理
	//--------------------------
	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理单元
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *Textrure3);//天空盒子映射
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	//cout << *Textrure1 << "z+z" << *Textrure2 << endl;
	glBindVertexArray(*VAO);


	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader)
{
	//绑定VAO对象
	glBindVertexArray(*VAO);

	//if (FirstMat4) 
	//{
	//	判断是否为第一次绘制，如果是，则将透视矩阵、视图矩阵初始化
	//	Load_mat4();
	//	FirstMat4 = false;
	//}
	//获取相应着色器对象并使用
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//在相应的纹理单元上绑定纹理
	//--------------------------
	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理单元
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *Textrure3);//天空盒子映射
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *Textrure4);
	//cout << *Textrure1 << "z+z" << *Textrure2 << endl;
	glBindVertexArray(*VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader, int _verticesCout)
{
	//绑定VAO对象
	glBindVertexArray(*VAO);

	//if (FirstMat4) 
	//{
	//	判断是否为第一次绘制，如果是，则将透视矩阵、视图矩阵初始化
	//	Load_mat4();
	//	FirstMat4 = false;
	//}
	//获取相应着色器对象并使用
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//在相应的纹理单元上绑定纹理
	//--------------------------
	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理单元
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *Textrure3);//天空盒子映射
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *Textrure4);
	//cout << *Textrure1 << "z+z" << *Textrure2 << endl;
	glBindVertexArray(*VAO);

	glDrawArrays(GL_TRIANGLES, 0, _verticesCout);
}
