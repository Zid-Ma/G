#pragma once
//��������ϵͳ����*(ͶӰ������ͼ���󣬼���ģʽ-p͸��-o����)
static void Load_matrix_(mat4* projection, mat4* view, char mod);
//����ģ�;���-����ŷ����*(ģ�;����ƶ�)
static void Load_matrix_model(mat4* model, vec3& translate_);
//����ģ�;���-����ŷ����*(ģ�;����ƶ�������)
static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_);
//����ģ�;���-����ŷ����*(ģ�;����ƶ������ţ���ת, ��ת�Ƕ�)
static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_, vec3 rotate_, float rotate_angle);

static void Load_matrix_(mat4* projection, mat4* view, char mod)
{
	switch(mod)
	{
		//͸��
		case 'p':
			*projection = glm::perspective(glm::radians(Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			*view = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
			break;
		//����
		case 'o':
			*projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);;
			*view = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
			break;
	}
}

static void Load_matrix_model(mat4* model, vec3& translate_)
{
	//��ʼ��
	*model = mat4(1.0f);
	//�ƶ�
	*model = translate(*model, translate_);
}

static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_)
{
	//��ʼ��
	*model = mat4(1.0f);
	//�ƶ�
	*model = translate(*model, translate_);
	//����
	*model = scale(*model, scale_);
}

static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_, vec3 rotate_, float rotate_angle)
{
	//��ʼ��
	*model = mat4(1.0f);
	//�ƶ�
	*model = translate(*model, translate_);
	//����
	*model = scale(*model, scale_);
	//��ת
	*model = glm::rotate(*model, rotate_angle, rotate_);
}
