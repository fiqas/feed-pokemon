#pragma once
class CollisionManager : public PhysicsActor {
public:

	void AddTags(String pathTag);

	CollisionManager(float x, float y ,  float sx, float sy, 
					 String pathTag, Vector2 side, String number);

	~CollisionManager(void);
	
	Actor *dish;
	float _x;
	float _y;
	bool empty;
	String _number;
	String _pick;
	void CreateEmptyDish();
	void FillDish(String pick);
	void EmptyDish();
	int pokemons_since_last_visited;
	float walked_since_last_visited;

};
