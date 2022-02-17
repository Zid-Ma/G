#pragma once
class Ray
{
private:
	bool first = true;
public:
	vec3 position;
	vec3 direction;//归一化后的方向向量
	float spacing = 0.1f;
	float maxDistance = 10.0f;
public:
	Ray()
	{

	}
	Ray(vec3 _position, vec3 _direction)
	{
		position = _position;
		direction = normalize(_direction);
	}
	Ray(vec3 _position, vec3 _direction, float _spacing, float _maxDistance)
	{
		position = _position;
		direction = normalize(_direction);
		spacing = _spacing;
		maxDistance = _maxDistance;
	}
	~Ray()
	{

	}
	/// <summary>
	/// 检测是否发生碰撞
	/// </summary>
	/// <param name="_layerCount">碰撞体图层号</param>
	/// <returns>碰撞体名称</returns>
	string CheckCollider(int _layerCount)
	{
		if (_layerCount > Collider_Layers::Collection.size())
			_layerCount = 0;
		int _count = maxDistance / spacing;//总共需要计算碰撞的次数
		vec3 _currentPosition;//射线当前位置
		_currentPosition = position;
		for (int i = 0; i < _count; i++)
		{
			_currentPosition += direction * spacing;
			//Print::Debug(to_string(Collider_Layers::Collection[_layerCount]->collection_entity.size()) + "");
			for (int x = 0; x < Collider_Layers::Collection[_layerCount]->collection_entity.size(); x++)
			{
				if (Collision(_currentPosition, Collider_Layers::Collection[_layerCount]->collection_entity[x]))
				{
					Print::Debug(Collider_Layers::Collection[_layerCount]->collection_entity[x]->name);
					return Collider_Layers::Collection[_layerCount]->collection_entity[x]->name;
				}
			}
		}
		return "";
	}
};