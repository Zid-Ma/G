#pragma once
//Åö×²¼ì²â
static bool Collision(vec3 _point, Collider_Coboid* _cc)
{
	if (_point.x > _cc->Point().x - (_cc->LWH().x * 0.5f))
	{
		if (_point.x < _cc->Point().x + (_cc->LWH().x * 0.5f))
		{
			if (_point.y > _cc->Point().y - (_cc->LWH().y * 0.5f))
			{
				if (_point.y < _cc->Point().y + (_cc->LWH().y * 0.5f))
				{
					if (_point.z > _cc->Point().z - (_cc->LWH().z * 0.5f))
					{
						if (_point.z < _cc->Point().z + (_cc->LWH().z * 0.5f))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
