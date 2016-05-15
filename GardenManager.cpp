#include "stdafx.h"


GardenManager::GardenManager(void) {

	LoadNeuralNetwork();

	theWorld.SetupPhysics(Vector2(0.0f, 0.0f));		// Ustawienie fizyki, potrzebne by Pikachu 
	theWorld.LoadLevel("pokemon_garden_level");	    // Wczytujemy plik ze statycznymi elementami œwiata tj. drzewa, p³ot oraz ziemia.
	
	pikachu = new Pikachu();
	AddPokemons();									//Funkcje dodaj¹ce odpowiednio kolzje do Pokemonów, managerów kolizji
	AddCollisionManager();
	
	theWorld.Add(pikachu);

	//Obsluga wyswietlanego tekstu
	text_screen = new FullScreenActor();
	text_screen->SetSprite("Resources/Images/text_002.png", 0, GL_CLAMP, GL_LINEAR);
	text_screen->SetLayer(10);
	
	text = new TextActor("Console", "", TXT_Left);
	text->SetPosition(Vector2(-9.0, -5.0));
	text->SetLayer(10);
	close_text_frame = true;

	theWorld.Add(text_screen);
	theWorld.Add(text);
	
	//Tworzenie grafu po którym bêdzie porusza³ siê Pikachu.
	BoundingBox bounds(Vector2(-20, -20), Vector2(20, 20));
	theSpatialGraph.CreateGraph( 1.0f, bounds);
	theSpatialGraph.EnableDrawGraph(true);

	//Pikachu reaguje na nastêpuj¹ce "wiadomoœci":
	theSwitchboard.SubscribeTo(this, "GoTo");
	theSwitchboard.SubscribeTo(this, "UpdateStatistics");
	theSwitchboard.SubscribeTo(this, "InspectDishes");

	counterPtr = &counter;
	*counterPtr = 0;

	pikachuOrder = theSound.LoadSample("Resources/Sounds/pikachuOrdered.mp3", false);
	acidSound = theSound.LoadSample("Resources/Sounds/acid.wav", false);

	pikachuOnAcid = new FullScreenActor();
	pikachuOnAcid->LoadSpriteFrames("Resources/Images/Animation/pikachuOnAcid_001.png", GL_CLAMP, GL_LINEAR);
	pikachuOnAcid->SetLayer(5);
	pikachuOnAcid->SetSpriteFrame(43);
	theWorld.Add(pikachuOnAcid);

	//Wylistuj wszystkie pokemony, dodaj do listy, by do nich podejsc
	pokemons = theTagList.GetObjectsTagged("pokemon");

	for(ActorSet::iterator itr1 = pokemons.begin(); itr1 != pokemons.end(); itr1++ ) {
		
		Actor* target = (*itr1);
		TargetList.push_back(target->GetName());
		std::cout << target->GetName() << std::endl;

	}

	std::cout << "Loaded targets" << std::endl;

}

GardenManager::~GardenManager(void) {

	delete pikachu;
	delete text;
	delete text_screen;
	delete counterPtr;
	delete pikachuOnAcid;

}

void GardenManager::LoadNeuralNetwork() {
	std::ifstream fin("Resources/NN_output.txt");

    String number_with_pick;
    float neural_output;

    while (fin >> number_with_pick >> neural_output)
    {
		NeuralNetwork[number_with_pick] = neural_output;
    }

}

bool GardenManager::CheckDish(String number, String pick) {
	String key = number + "_" + pick;
	if(NeuralNetwork[key] < 0.5) return true;
	else return false;
}

void GardenManager::InspectDishes() {
	for(ActorSet::iterator itr1 = pokemons.begin(); itr1 != pokemons.end(); itr1++ ) {
		Actor* customer_temp = (*itr1);
		CollisionManager* customer;
		customer = dynamic_cast < CollisionManager* >(customer_temp);
		
		if(CheckDish(customer->_number, customer->_pick) && customer->IsTagged("JE")) {
			customer->Untag("JE");
			customer->Tag("CZEKA_NA_RACHUNEK");
			std::cout << "SIEC " << customer->GetName() << std::endl; 
		}

	}

}



//Tutaj bedzie funkcja wybierajaca do ktorego Pokemona ma podejsc.
Actor* GardenManager::ChooseTarget() {

	Actor* target;
	return target;
}

void GardenManager::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button) {
	
	if(button == MOUSE_LEFT) {
		if (*counterPtr < TargetList.size()) {
			Analyze(*counterPtr);
		}

		else {

			pikachuOnAcid -> PlaySpriteAnimation(0.1f, SAT_OneShot, 0, 43, "drugs");
			theSound.PlaySound(acidSound);
			*counterPtr = 0;

		}

		*counterPtr = *counterPtr +1;

	}
	
}

void GardenManager::ReceiveMessage(Message* message) {


	if (message->GetMessageName() == "GoTo" ) { 

		TypedMessage<Vector2> *m = (TypedMessage<Vector2>*)message;
		Vector2 destinationCoordinates = m->GetValue();
		pikachu->GoTo(destinationCoordinates);
	}

	if (message->GetMessageName() == "UpdateStatistics" ) { 
		UpdatePokemonStatistics();
	}

	if (message->GetMessageName() == "InspectDishes" ) { 
		InspectDishes();
	}
	
}

void GardenManager::UpdatePokemonStatistics() {
	for(ActorSet::iterator itr1 = pokemons.begin(); itr1 != pokemons.end(); itr1++ ) {
		Actor* waiting_customer_temp = (*itr1);
		CollisionManager* waiting_customer;
		waiting_customer = dynamic_cast < CollisionManager* >(waiting_customer_temp);
		waiting_customer->pokemons_since_last_visited += 1;
		waiting_customer->walked_since_last_visited += pikachu->distanceWalked;

		//Tutaj jest warunek, ktory powoduje, ze pokemon zjadl i talerz zamienia sie na pusty.
		//Narazie na chama robi, ze jak Pikachu odwiedzi 3 pokemony po pokemonie X, to Pokemon X zdazyl zjesc.
		if(waiting_customer->pokemons_since_last_visited >= 3 && waiting_customer->IsTagged("JE")) {
			std::cout << "CZYSZCZE TALERZ POKEMONA " << waiting_customer->GetName() << std::endl;
			waiting_customer->EmptyDish();
		}

	}
	theSwitchboard.Broadcast(new Message("InspectDishes"));

}

void GardenManager::Analyze(int toDo) {
	
	ActorSet named_pokemon = theTagList.GetObjectsTagged(TargetList[toDo]);

	//FindTaggedPokemons(pokemons, SentencesList[toDo].noun, "CZEKA_NA_ZAMOWIENIE");

	theSound.PlaySound(pikachuOrder);

	ActorSet::iterator itr1 = named_pokemon.begin();
	Actor* visited_customer_temp = (*itr1);
	CollisionManager* visited_customer = dynamic_cast < CollisionManager* >(visited_customer_temp);
	TypedMessage<Vector2> *m = new TypedMessage<Vector2>("GoTo", visited_customer->_side);
	theSwitchboard.Broadcast(m);
	std::cout << "GOING TO " << visited_customer->GetName() << std::endl;
	pikachu->customer_served = visited_customer;
	
}

void GardenManager::Text(String display_text) {

	text_screen->SetSprite("Resources/Images/text_001.png");
	text->SetDisplayString(display_text);

}

void GardenManager::CloseText() {

	text_screen->SetSprite("Resources/Images/text_002.png");
	text->SetDisplayString("");

}

//void GardenManager::FindTaggedPokemons(ActorSet& bothTaggedActors, String adjective, String noun) {
//
//	ActorSet adjectiveTaggedActors;
//	ActorSet nounTaggedActors;
//
//	if(!adjective.empty()) { //je¿eli przymiotnik istnieje i string nie jest pusty
//
//		adjectiveTaggedActors = theTagList.GetObjectsTagged(adjective);
//
//		if(noun.empty()) bothTaggedActors = theTagList.GetObjectsTagged(adjective);
//		
//	} 
//
//	if(!noun.empty()) { //je¿eli rzeczownik istnieje i string nie jest pusty
//
//		nounTaggedActors = theTagList.GetObjectsTagged(noun);
//	
//		if(adjective.empty()) bothTaggedActors = theTagList.GetObjectsTagged(noun);
//
//	}
//
//	for(ActorSet::iterator itr1 = nounTaggedActors.begin(); itr1 != nounTaggedActors.end(); itr1++ ) {
//
//		if	(nounTaggedActors.empty() || adjectiveTaggedActors.empty()) break;
//	
//		else if (nounTaggedActors.empty() && adjectiveTaggedActors.empty()) break;
//
//		for(ActorSet::iterator itr2 = adjectiveTaggedActors.begin(); itr2 != adjectiveTaggedActors.end(); itr2++ ) {
//
//			Actor* targetActor1 = (*itr1);
//			Actor* targetActor2 = (*itr2);
//
//			if(targetActor1->GetName() == targetActor2->GetName()) bothTaggedActors.insert(targetActor2);
//
//		}
//		
//	}
//	
//}

void GardenManager::Update(float dt) {

	GameManager::Update(dt);

}

void GardenManager::CreatePokemon(float x, float y , float sizex, float sizey, String pathName) {

	theWorld.Add(new Pokemon(x, y, sizex, sizey, pathName));

}

void GardenManager::CreateCollisionManager(float x, float y , float sx, float sy, String pathTag, Vector2 side, String number) {

	theWorld.Add(new CollisionManager(x, y , sx, sy, pathTag, side, number));

}

void GardenManager::AddCollisionManager() {

	CreateCollisionManager(-11.0f, -7.5f, 1.0f, 0.1f, "Resources/Tags/jigglypuff_tags.txt", Vector2(-9.16f, -6.91f), "001");
	CreateCollisionManager(3.0f, 7.5f, 1.0f, 0.1f,  "Resources/Tags/victreebel_tags.txt", Vector2(1.55f, 7.92f), "002");
	CreateCollisionManager(8.0f, 4.8f, 1.5f, 0.1f, "Resources/Tags/arbok_tags.txt", Vector2(6.37f, 5.7f), "003");
	CreateCollisionManager(4.0f, -4.5f, 1.0f, 0.1f, "Resources/Tags/bulbasaur_tags.txt", Vector2( 2.28f, -4.29f), "004");
	CreateCollisionManager(-4.0f, 7.3f, 1.0f, 0.1f,  "Resources/Tags/psyduck_tags.txt", Vector2(-5.4f,8.06f), "005");
	CreateCollisionManager(11.0f, -5.5f, 1.0f, 0.1f, "Resources/Tags/slowpoke_tags.txt", Vector2(9.64f, -5.0f), "006");
	CreateCollisionManager(-5.6f, 2.35f, 1.0f, 0.1f, "Resources/Tags/koffing_tags.txt", Vector2( -4.25f, 2.49f), "007");
	CreateCollisionManager(5.9f, -0.6f, 1.0f, 0.1f, "Resources/Tags/snorlax_tags.txt", Vector2(6.04f, -0.12f), "008");
	CreateCollisionManager(-10.26f, -1.3f, 1.0f, 0.1f, "Resources/Tags/charizard_tags.txt", Vector2(-7.28f, -0.8f), "009");
	CreateCollisionManager(9.0f, -1.5f, 1.0f, 0.1f, "Resources/Tags/squirtle_tags.txt", Vector2(7.58, -1.07f), "010");
	CreateCollisionManager(-0.5f, 7.0f, 1.0f, 0.1f,  "Resources/Tags/meowth_tags.txt", Vector2( -1.66f, 7.4f), "011");
	CreateCollisionManager(6.0f, 2.5f, 1.0f, 0.1f, "Resources/Tags/pidgeot_tags.txt", Vector2(7.17f, 3.04f), "012");
	CreateCollisionManager(-5.0f, -5.7f, 1.0f, 0.1f, "Resources/Tags/raichu_tags.txt", Vector2(-6.15f, -5.15f), "013");
	CreateCollisionManager(-5.86f,0.2f ,1.0f, 0.1f, "Resources/Tags/wobbuffet_tags.txt", Vector2 (-7.09f,0.34f), "014");
	CreateCollisionManager(-0.5f, -4.9f, 1.0f, 0.1f, "Resources/Tags/mew_tags.txt", Vector2 (-1.9f,-4.46f), "015");
	CreateCollisionManager( -9.46f,-5.3f , 1.0f, 0.1f , "Resources/Tags/mewtwo_tags.txt", Vector2 (-11.26f,-4.5f), "016");
	CreateCollisionManager( -6.84, 5.02f, 1.0f, 0.1f , "Resources/Tags/gyarados_tags.txt", Vector2 (-8.86f,5.9f), "001");
	CreateCollisionManager( 2.81f, -7.58f, 1.0f, 0.1f , "Resources/Tags/scyther_tags.txt", Vector2 (0.68f,-6.78f), "002");
	CreateCollisionManager( 10.58, 2.57f, 1.0f, 0.1f , "Resources/Tags/staryu_tags.txt", Vector2 (9.65f,2.98f), "003");
	CreateCollisionManager( 9.41, -7.7f, 1.0f, 0.1f , "Resources/Tags/licktung_tags.txt", Vector2 (7.4f,-7.2f), "004");
	CreateCollisionManager( -10.8f, 2.3f, 1.0f, 0.1f , "Resources/Tags/muk_tags.txt", Vector2 (-9.3f,2.82f), "005");
	CreateCollisionManager( 2.29f, 3.17f, 1.0f, 0.1f , "Resources/Tags/alakazam_tags.txt", Vector2 (3.9f,3.69f), "006");
	CreateCollisionManager( -4.4f, -8.22f, 1.0f, 0.1f , "Resources/Tags/ninetales_tags.txt",Vector2 (-3.1f,-8.1f), "007");
	CreateCollisionManager( 8.0f, -3.42f, 1.0f, 0.1f , "Resources/Tags/caterpie_tags.txt", Vector2 (9.2f,-2.9f), "008");

}

void GardenManager::AddPokemons() {

	CreatePokemon(-11.0f, -7.0f, 1.0f, 1.0f, "Resources/Images/pokemons/jigglypuff.png" );
	CreatePokemon(3.0f, 8.0f, 1.5f, 1.8f, "Resources/Images/pokemons/victreebel.png");
	CreatePokemon(8.0f, 6.0f, 2.1f, 2.5f, "Resources/Images/pokemons/arbok.png");
	CreatePokemon(4.0f, -4.0f, 1.5f, 1.5f, "Resources/Images/pokemons/bulbasaur.png");
	CreatePokemon(-4.0f, 8.0f, 1.5f, 1.5f, "Resources/Images/pokemons/psyduck.png");
	CreatePokemon(11.0f, -5.0f, 1.5f, 1.2f, "Resources/Images/pokemons/slowpoke.png");
	CreatePokemon(-5.5f, 2.7f, 1.6f, 1.5f, "Resources/Images/pokemons/koffing.png");
	CreatePokemon(6.0f, 0.0f, 1.8f, 2.0f, "Resources/Images/pokemons/snorlax.png");
	CreatePokemon(-10.0f, 0.0f, 3.2f, 3.3f, "Resources/Images/pokemons/charizard_001.png");
	CreatePokemon(9.0f, -1.0f, 1.2f, 1.3f, "Resources/Images/pokemons/squirtle_001.png");
	CreatePokemon(-0.5f, 7.5f, 1.4f, 1.5f, "Resources/Images/pokemons/meowth_001.png");
	CreatePokemon(6.0f, 3.0f, 1.0f, 1.5f, "Resources/Images/pokemons/pidgeot_001.png");
	CreatePokemon(-5.0f, -5.0f, 1.8f, 1.6f, "Resources/Images/pokemons/raichu_001.png");
	CreatePokemon(-5.8f, 0.5f, 2.0f, 2.0f, "Resources/Images/pokemons/wobbuffet.png");
	CreatePokemon(-0.5f, -4.5f, 2.5f, 2.5f, "Resources/Images/pokemons/mew.png");
	CreatePokemon(-9.5f, -4.3f,  3.0f, 3.0f, "Resources/Images/pokemons/mewtwo.png");
	CreatePokemon(-7.17f, 5.8f,  2.5f, 2.5f, "Resources/Images/pokemons/gyarados.png");
	CreatePokemon(2.4f, -6.73f,  3.5f, 3.5f, "Resources/Images/pokemons/scyther.png");
	CreatePokemon(10.7f, 2.9f,  2.0f, 2.0f, "Resources/Images/pokemons/staryu.png");
	CreatePokemon(9.3f, -7.3f,  2.5f, 2.5f, "Resources/Images/pokemons/licktung.png");
	CreatePokemon(-10.8f, 3.04f,  2.0f, 2.0f, "Resources/Images/pokemons/muk.png");
	CreatePokemon(2.5f, 3.8f, 2.0f, 2.0f, "Resources/Images/pokemons/alakazam.png");
	CreatePokemon(-4.5f, -7.8f, 2.0f, 2.0f, "Resources/Images/pokemons/ninetales.png");
	CreatePokemon(8.1f, -2.9f, 3.0f, 3.0f, "Resources/Images/pokemons/caterpie.png");

}
