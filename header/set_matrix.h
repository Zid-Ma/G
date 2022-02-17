#pragma once
//加载坐标系统矩阵*(投影矩阵，视图矩阵，加载模式-p透视-o正交)
static void Load_matrix_(mat4* projection, mat4* view, char mod);
//加载模型矩阵-基于欧拉角*(模型矩阵，移动)
static void Load_matrix_model(mat4* model, vec3& translate_);
//加载模型矩阵-基于欧拉角*(模型矩阵，移动，缩放)
static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_);
//加载模型矩阵-基于欧拉角*(模型矩阵，移动，缩放，旋转, 旋转角度)
static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_, vec3 rotate_, float rotate_angle);

static void Load_matrix_(mat4* projection, mat4* view, char mod)
{
	switch(mod)
	{
		//透视
		case 'p':
			*projection = glm::perspective(glm::radians(Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			*view = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
			break;
		//正交
		case 'o':
			*projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);;
			*view = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
			break;
	}
}

static void Load_matrix_model(mat4* model, vec3& translate_)
{
	//初始化
	*model = mat4(1.0f);
	//移动
	*model = translate(*model, translate_);
}

static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_)
{
	//初始化
	*model = mat4(1.0f);
	//移动
	*model = translate(*model, translate_);
	//缩放
	*model = scale(*model, scale_);
}

static void Load_matrix_model(mat4* model, vec3 translate_, vec3 scale_, vec3 rotate_, float rotate_angle)
{
	//初始化
	*model = mat4(1.0f);
	//移动
	*model = translate(*model, translate_);
	//缩放
	*model = scale(*model, scale_);
	//旋转
	*model = glm::rotate(*model, rotate_angle, rotate_);
}
