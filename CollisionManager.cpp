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
	SetWillEatAfter();
	SetWillOrderAfter();
	InitPhysics();


	dish = new Actor();
	exclamation = new Actor();
	_side = side;
	_x = x;
	_y = y;
	_number = number;
	_pick = "e";
	AddTags(pathTag);
	Tag("CZEKA_NA_KELNERA");
	CreateEmptyDish();
	CreateExclamation();
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
	dish->SetAlpha(1.0f);
	dish->LoadSpriteFrames(path_name, GL_CLAMP, GL_LINEAR);
	dish->SetSpriteFrame(0);
}

void CollisionManager::EmptyDish() {
	String path_name = "Resources/Images/dishes/" + _number + "_e.png";
	dish->SetAlpha(1.0f);
	dish->LoadSpriteFrames(path_name, GL_CLAMP, GL_LINEAR);
	dish->SetSpriteFrame(0);
	_pick = "e";
}

 void CollisionManager::HideDish() {
	dish->SetAlpha(0.0f);
 }

void CollisionManager::CreateExclamation() {
	String path_name = "Resources/Images/exclamation.png";
	exclamation->SetLayer(4);
	exclamation->SetSize(1,1);
	exclamation->SetPosition(_x ,_y+2);
	exclamation->LoadSpriteFrames(path_name, GL_CLAMP, GL_LINEAR);
	exclamation->SetSpriteFrame(0);
	exclamation->SetAlpha(1.0f);
}

void CollisionManager::ShowExclamation() {
	exclamation->SetAlpha(1.0f);
}

void CollisionManager::HideExclamation() {
	exclamation->SetAlpha(0.0f);
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

void CollisionManager::SetWillEatAfter() {
	CollisionManager::will_eat_after = rand() % 5 + 1;
}

void CollisionManager::SetWillOrderAfter() {
	CollisionManager::will_order_after = rand() % 5 + 3;
}

CollisionManager::~CollisionManager(void)
{
}
