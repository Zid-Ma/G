#pragma once

static void Load_draw(GLuint* VAO, Shader& shader);
//����ͼ��(��������������������������ɫ��)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, Shader& shader);
//����ͼ��(�����������������������󣬾��淴�����������ɫ��)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, Shader& shader);
//����ͼ��(�����������������������󣬾��淴��������󣬻���ӳ�����������ɫ��)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, Shader& shader);
//����ͼ��(�����������������������󣬾��淴��������󣬻���ӳ��������󣬷�����ͼ���������ɫ��)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader);
//����ͼ��(�����������������������󣬾��淴��������󣬻���ӳ��������󣬷�����ͼ���������ɫ��, ��������)
static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader, int _verticesCout);

static void Load_draw_multiple(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, Shader& shader);

static void Load_draw(GLuint* VAO, Shader& shader)
{
	//��VAO����
	glBindVertexArray(*VAO);

	//��ȡ��Ӧ��ɫ������ʹ��
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, Shader& shader)
{
	//��VAO����
	glBindVertexArray(*VAO);

	//��ȡ��Ӧ��ɫ������ʹ��
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//����Ӧ������Ԫ�ϰ�����
	//--------------------------
	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//������Ԫ
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ�����輤���κ�����Ԫ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//��պ���ӳ��
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, Shader& shader)
{
	//��VAO����
	glBindVertexArray(*VAO);

	//��ȡ��Ӧ��ɫ������ʹ��
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//����Ӧ������Ԫ�ϰ�����
	//--------------------------
	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//������Ԫ
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ�����輤���κ�����Ԫ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//��պ���ӳ��
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint *VAO, GLuint *Textrure1, GLuint* Textrure2, GLuint* Textrure3, Shader &shader)
{
	//��VAO����
	glBindVertexArray(*VAO);

	//if (FirstMat4) 
	//{
	//	�ж��Ƿ�Ϊ��һ�λ��ƣ�����ǣ���͸�Ӿ�����ͼ�����ʼ��
	//	Load_mat4();
	//	FirstMat4 = false;
	//}
	//��ȡ��Ӧ��ɫ������ʹ��
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model",Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//����Ӧ������Ԫ�ϰ�����
	//--------------------------
	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//������Ԫ
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ�����輤���κ�����Ԫ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *Textrure3);//��պ���ӳ��
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	//cout << *Textrure1 << "z+z" << *Textrure2 << endl;
	glBindVertexArray(*VAO);


	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader)
{
	//��VAO����
	glBindVertexArray(*VAO);

	//if (FirstMat4) 
	//{
	//	�ж��Ƿ�Ϊ��һ�λ��ƣ�����ǣ���͸�Ӿ�����ͼ�����ʼ��
	//	Load_mat4();
	//	FirstMat4 = false;
	//}
	//��ȡ��Ӧ��ɫ������ʹ��
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//����Ӧ������Ԫ�ϰ�����
	//--------------------------
	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//������Ԫ
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ�����輤���κ�����Ԫ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *Textrure3);//��պ���ӳ��
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *Textrure4);
	//cout << *Textrure1 << "z+z" << *Textrure2 << endl;
	glBindVertexArray(*VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void Load_draw(GLuint* VAO, GLuint* Textrure1, GLuint* Textrure2, GLuint* Textrure3, GLuint* Textrure4, Shader& shader, int _verticesCout)
{
	//��VAO����
	glBindVertexArray(*VAO);

	//if (FirstMat4) 
	//{
	//	�ж��Ƿ�Ϊ��һ�λ��ƣ�����ǣ���͸�Ӿ�����ͼ�����ʼ��
	//	Load_mat4();
	//	FirstMat4 = false;
	//}
	//��ȡ��Ӧ��ɫ������ʹ��
	shader.use();
	shader.setMat4("projection", Projection_Mat4);
	shader.setMat4("view", View_Mat4);
	shader.setMat4("model", Model_Mat4);
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);

	//����Ӧ������Ԫ�ϰ�����
	//--------------------------
	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//������Ԫ
	glBindTexture(GL_TEXTURE_2D, *Textrure1);//����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ�����輤���κ�����Ԫ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *Textrure2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *Textrure3);//��պ���ӳ��
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *Textrure4);
	//cout << *Textrure1 << "z+z" << *Textrure2 << endl;
	glBindVertexArray(*VAO);

	glDrawArrays(GL_TRIANGLES, 0, _verticesCout);
}
