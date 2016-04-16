#pragma once
class Object : public PhysicsActor
{
public:
	Object(float x, float y , String pathName, float sx, float sy);
	~Object(void);
};

