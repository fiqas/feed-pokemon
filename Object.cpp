#include "stdafx.h"
#include "Object.h"


Object::Object(float x, float y , String pathName, float sx, float sy)
{
	SetLayer(3);
	SetPosition(x, y);
	SetName("object");
	SetSprite(pathName);
	SetSize(sx, sy);
	SetDensity(0.0f);
	SetFriction(0.0f);
	SetRestitution(0.1f);
	InitPhysics();
}


Object::~Object(void)
{
}
