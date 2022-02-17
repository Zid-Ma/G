#pragma once
class Ray
{
private:
	bool first = true;
public:
	vec3 position;
	vec3 direction;//��һ����ķ�������
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
	/// ����Ƿ�����ײ
	/// </summary>
	/// <param name="_layerCount">��ײ��ͼ���</param>
	/// <returns>��ײ������</returns>
	string CheckCollider(int _layerCount)
	{
		if (_layerCount > Collider_Layers::Collection.size())
			_layerCount = 0;
		int _count = maxDistance / spacing;//�ܹ���Ҫ������ײ�Ĵ���
		vec3 _currentPosition;//���ߵ�ǰλ��
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