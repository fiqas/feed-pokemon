#pragma once
#include "stdafx.h"

//centrum dowodzenia gr¹

class GardenManager : public GameManager, public MouseListener {

public:

	GardenManager(void);
	virtual ~GardenManager(void);
	
	void CreatePokemon(float x, float y , float sizex, float sizey, String pathName);
	void CreateCollisionManager(float x, float y ,  float sx, float sy, String pathTag, Vector2 side, String number);
	void AddCollisionManager();
	void AddPokemons();
	void Update(float dt);
	void ReceiveMessage(Message* message);
	void Text(String display_text);
	void CloseText();
	
	virtual void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);

	void Analyze(int toDo);
	Actor* ChooseTarget();
	void UpdatePokemonStatistics();
	void InspectDishes();
	bool CheckDish(String number, String pick);
	void LoadNeuralNetwork();

	Pikachu* pikachu;
	ActorSet pokemons;
	Actor* concretepokemon;
	FullScreenActor* text_screen;
	FullScreenActor* pikachuOnAcid;
	TextActor* text;
	bool close_text_frame;
	std::map<String, float> NeuralNetwork;

	AngelSampleHandle pikachuOrder;
	AngelSampleHandle pikachuQuestion;
	AngelSampleHandle pikachuLaughing;
	AngelSampleHandle acidSound;
	
	int counter;
	int* counterPtr;

	std::vector <String> TargetList;


	String sentence;

};