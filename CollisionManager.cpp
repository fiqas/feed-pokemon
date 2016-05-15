#include "stdafx.h"

CollisionManager::CollisionManager(float x, float y ,  float sx, float sy, String pathTag, Vector2 side, String number) {

	SetLayer(6);
	SetPosition(x, y);
	SetSize(sx, sy);
	SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	SetColor(0.0f, 0.0f, 0.0f, 0.0f);
	SetDensity(0.0f);
	SetFriction(0.0f); 
	SetRestitution(0.1f);
	InitPhysics();


	dish = new Actor();
	_side = side;
	_x = x;
	_y = y;
	_number = number;
	_pick = "e";
	AddTags(pathTag);
	Tag("CZEKA_NA_KELNERA");
	CreateEmptyDish();
	empty = true;
	pokemons_since_last_visited = 0;
	walked_since_last_visited = 0;

}

void CollisionManager::CreateEmptyDish() {
	String path_name = "Resources/Images/dishes/" + _number + "_e.png";
	dish->SetLayer(4);
	dish->SetSize(1, 1);
	dish->SetPosition(_x ,_y);
	dish->LoadSpriteFrames(path_name, GL_CLAMP, GL_LINEAR);
	dish->SetSpriteFrame(0);
}

void CollisionManager::FillDish(String pick) {
	_pick = pick;
	String path_name = "Resources/Images/dishes/" + _number + "_" + pick + ".png";
	dish->LoadSpriteFrames(path_name, GL_CLAMP, GL_LINEAR);
	dish->SetSpriteFrame(0);
}

void CollisionManager::EmptyDish() {
	String path_name = "Resources/Images/dishes/" + _number + "_e.png";
	dish->LoadSpriteFrames(path_name, GL_CLAMP, GL_LINEAR);
	dish->SetSpriteFrame(0);
	_pick = "e";
}

void CollisionManager::AddTags(String pathTag) {

	std::vector<String> tags;
	std::fstream file;
	file.open(pathTag, std::ios::in);

	if( file.good() ) {
		
		String tag;

		while ( !file.eof() ) {

			getline(file, tag);
			tags.push_back(tag);

		}
		
		file.close();
	
	}	
	
	else std::cout << "Err! file : " << pathTag << " not found."  << std::endl;

	SetName(tags[0]);

	for (int i = 0; i < tags.size(); i++) {
		
		Tag(tags[i]);

	}

}


CollisionManager::~CollisionManager(void)
{
}
