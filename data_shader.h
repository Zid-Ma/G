#pragma once
class Shaders
{
private:
	static Shader pointLight;
	Shader mo;
public:
	Shaders()
	{
		pointLight = Shader(Path_Shader + "cube.vert", Path_Shader + "light.frag");
	}
public:
	void Use_PointLight(vec4 _color)
	{
		pointLight.use();
		pointLight.setVec4("color", _color);
	}
};
Shader Shaders::pointLight;