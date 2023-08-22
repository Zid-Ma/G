#pragma once
static float MiniMap_Width = 0;
static float MiniMap_Height = 0;
static vec2 MiniMap_Position;
static vec2 MiniMap_Point1;
static vec2 MiniMap_Point2;
static vec2 MiniMap_Point3;
static vec2 MiniMap_Point4;

static void MiniMapChange(vec2 _position,float _width)
{
	if (distance(MiniMap_Position.x, 0.0f) < 400 && distance(MiniMap_Position.x, 0.0f) < 400)
	{
		MiniMap_Position = vec2(_position.x * 0.002f, _position.y * 0.002f);
		MiniMap_Width = _width;
		MiniMap_Height = _width;
	}
	MiniMap_Point1 = vec2(MiniMap_Position.x - (MiniMap_Width * 0.5f) + (MiniMap_Width * 0.5f), MiniMap_Position.y + (MiniMap_Height * 0.5f) + (MiniMap_Height * 0.5f));
	MiniMap_Point2 = vec2(MiniMap_Position.x + (MiniMap_Width * 0.5f) + (MiniMap_Width * 0.5f), MiniMap_Position.y + (MiniMap_Height * 0.5f) + (MiniMap_Height * 0.5f));
	MiniMap_Point3 = vec2(MiniMap_Position.x - (MiniMap_Width * 0.5f) + (MiniMap_Width * 0.5f), MiniMap_Position.y - (MiniMap_Height * 0.5f) + (MiniMap_Height * 0.5f));
	MiniMap_Point4 = vec2(MiniMap_Position.x + (MiniMap_Width * 0.5f) + (MiniMap_Width * 0.5f), MiniMap_Position.y - (MiniMap_Height * 0.5f) + (MiniMap_Height * 0.5f));
}