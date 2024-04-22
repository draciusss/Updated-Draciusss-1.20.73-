#pragma once

class AABBShapeComponent
{
public:
	Vector3<float> PosLower = { 0, 0, 0 }; //0x0000
	Vector3<float> PosUpper = { 0, 0, 0 }; //0x000C
	Vector2<float> Hitbox = { 0, 0 }; //0x0018 
};