﻿#include "stdafx.h"
#include "Pikachu.h"
#include <cstdlib>

Pikachu::Pikachu(void) {

	//ustawienia pikachu
	SetName("Pikachu");
	SetPosition(0,0);
	LoadSpriteFrames("Resources/Images/pikachu/pikachu_001.png", GL_CLAMP, GL_LINEAR);
	SetSpriteFrame(2);
	SetLayer(3);
	last_mode = "CZEKA_NA_KELNERA";

	walkingright = false;
	walkingleft = false;
	walkingup = false;
	walkingdown = false;

	_pathIndex = 0;

	actioner = new FullScreenActor();

	chat_screen = new FullScreenActor();
	chat_screen->SetSprite("Resources/Images/text_002.png", 0, GL_CLAMP, GL_LINEAR);
	chat_screen->SetLayer(10);

	chat = new TextActor("Console", "", TXT_Left);
	chat->SetPosition(Vector2(-9.0, -5.0));
	chat->SetLayer(10);

	theWorld.Add(chat_screen);
	theWorld.Add(chat);

	//Pikachu reaguje na dane komunikaty:
	theSwitchboard.SubscribeTo(this, "GoingLeft");
	theSwitchboard.SubscribeTo(this, "GoingRight");
	theSwitchboard.SubscribeTo(this, "GoingUp");
	theSwitchboard.SubscribeTo(this, "GoingDown");
	theSwitchboard.SubscribeTo(this, "PathPointReached");  //Osi¹gniêcie wierzcho³ka grafu
	theSwitchboard.SubscribeTo(this, "EndPointReached");   //Zakoñczenie wyznaczonej œcie¿ki

	happyPikachuSound = theSound.LoadSample("Resources/Sounds/happyPikachuSound.mp3", false);
	sadPikachuSound = theSound.LoadSample("Resources/Sounds/sadPikachuSound.wav", false);
	battleSound = theSound.LoadSample("Resources/Sounds/battleSceneEpicMusic.wav", false);
	pikachuTalking = theSound.LoadSample("Resources/Sounds/pikachuTalk.mp3", false);

}

Pikachu::~Pikachu(void) {

	delete chat;
	delete chat_screen;
	delete actioner;

}

void Pikachu::GoTo(Vector2 newDestination) { 

	//Funkcja tworz¹ca œcie¿kê.
	Vector2List pathTest;
	theSpatialGraph.GetPath(GetPosition(), newDestination, pathTest);
	distanceWalked = 0;

	if (pathTest.size() > 0) {

		_pathPoints = pathTest;
		_pathIndex = 0;

		GetToNextPoint();

	}

}

void Pikachu::GetToNextPoint() { 

	//Ta funkcja jest odpowiedzialna za puszczanie odpowiedniej animacji
	//oraz za samo chodzenie po wierzcho³kach grafu.

	Vector2 next = _pathPoints[++_pathIndex];
	distance = Vector2::Distance(_position, next);
	distanceWalked += distance;
	time = distance / 4.0f;
	angle = Angle(Vector2(10,0), Vector2(next.X - GetPosition().X, next.Y - GetPosition().Y));	

	//Opracowanie w któr¹ stronê Pikachu wykona ruch.

	if ((angle >= 0 && angle < 30) || (angle <= 360 && angle > 330)) theSwitchboard.Broadcast(new Message("GoingRight"));
	else if (angle >= 30 && angle < 150) theSwitchboard.Broadcast(new Message("GoingUp"));
	else if (angle >= 150 && angle < 210) theSwitchboard.Broadcast(new Message("GoingLeft"));
	else if (angle >= 210 && angle <= 330) theSwitchboard.Broadcast(new Message("GoingDown"));
	else theSwitchboard.Broadcast(new Message("Standing"));

	MoveTo(next, time, false, "PathPointReached");

	currentPosition = GetPosition();

}

double Pikachu::Angle(Vector2 position, Vector2 destination) {

	//Funkcja obliczaj¹ca k¹t miêdzy dwoma wektorami.

	scalar = position.X * destination.X + position.Y * destination.Y;
    positionlength = sqrt(pow(position.X, 2) + pow(position.Y, 2));
    destinationlength = sqrt(pow(destination.X, 2) + pow(destination.Y, 2));
    cosinus = scalar / (positionlength * destinationlength);

	if ( destination.Y < position.Y)  angle = 360 - acos(cosinus) * 180 / M_PI;
    else angle = acos(cosinus) * 180 / M_PI;
	
	return angle;

}


void Pikachu::Update(float dt) {	

	Actor::Update(dt);

}

void Pikachu::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button) {
	

	if(button == MOUSE_RIGHT) {

		chat_screen->SetSprite("Resources/Images/text_002.png", 0, GL_CLAMP, GL_LINEAR);
		chat->SetDisplayString("");
	}

}

void Pikachu::Proposition(String customer_name){
	String command = "copy /Y Resources\\Tags\\" + customer_name + "_tags.txt pokemon.csv" ;
	const char* conv = command.c_str();
	std::cout << conv << std::endl;
	system(conv);
	system("python program.py");
	
}

void Pikachu::Talk() {

	theSound.PlaySound(pikachuTalking);
	pikachuDialoguePokemon = theSound.LoadSample("Resources/Sounds/" + pokemonName + ".wav", false);
	theSound.PlaySound(pikachuDialoguePokemon);

	if(customer_served->IsTagged("CZEKA_NA_KELNERA")) {
		last_mode = "CZEKA_NA_KELNERA";
		customer_served->FillDish("1");
		customer_served->Untag("CZEKA_NA_KELNERA");
		Proposition(customer_served->GetName());
		
		std::fstream file;
		file.open("result", std::ios::in);
		String result;

		if( file.good() ) {
			while ( !file.eof() ) {
				getline(file, result);
			}
			file.close();
		}	

		String proposition = customer_served->GetName() + " : " + result ;
		chat_screen->SetSprite("Resources/Images/text_001.png", 0.5, GL_CLAMP, GL_LINEAR);
		chat_screen->SetAlpha(1.0f);
		chat_screen->SetLayer(10);
		chat->SetDisplayString(proposition);

		customer_served->Tag("JE");
	}
	else if(customer_served->IsTagged("CZEKA_NA_RACHUNEK")) {
		last_mode = "CZEKA_NA_RACHUNEK";
		customer_served->Untag("CZEKA_NA_RACHUNEK");
		customer_served->Tag("ODPOCZYWA");
		customer_served->HideDish();
	}

	customer_served->HideExclamation();
	

	customer_served->pokemons_since_last_visited = 0;
	customer_served->walked_since_last_visited = 0;
	theSwitchboard.Broadcast(new Message("UpdateStatistics"));
	std::cout << "talk" << std::endl;
}

void Pikachu::ReceiveMessage(Message* message) {

	if ( (message->GetMessageName() == "PathPointReached") && (message->GetSender() == this) ) { 

		//Zosta³a odebrana wiadomoœæ o dotarciu do wierzcho³ka grafu, ktory jest zawarty w naszej œcie¿ce.
		
		if (_pathIndex < _pathPoints.size() - 1) GetToNextPoint();

		else {

			//Kiedy przeszliœmy przez wszystkie wierzcho³ki na naszej œcie¿ce.
			theSwitchboard.Broadcast(new Message("EndPointReached", this));
			_pathPoints.clear();
			_pathIndex = 0;
			std::cout << distanceWalked << std::endl;

			Talk();

		}

	}

	else if (message->GetMessageName() == "EndPointReached") {
			
			//Kiedy dotrzemy do celu warto by³oby zastopowaæ animacjê, w tym celu ustawiona zostaje odpowiednia klatka
			//w zale¿noœci od kierunku poruszania siê postaci.
			//Pikachu niestety nie mieli nó¿kami w czasie przechodzenia po grafie,
			//dopiero robi to po zatrzymaniu siê. Dziêki temu nie mieli nó¿kami w nieskoñczonoœæ.

		if (walkingright) SetSpriteFrame(13);						
		else if (walkingleft) SetSpriteFrame(9);
		else if (walkingup) SetSpriteFrame(5);
		else SetSpriteFrame(1);		

	}

	// Poni¿sze funkcje ustawiaj¹ wartoœci true/false dotycz¹ce kierunku poruszania siê postaci
	// a tak¿e graj¹ odpowiedni¹ animacjê.

	else if (message->GetMessageName() == "GoingRight") {

		walkingright = true;
		walkingleft = false;
		walkingup = false;
		walkingdown = false;
		PlaySpriteAnimation(time, SAT_OneShot, 12, 15, "WalkingRight");

	}

	else if (message->GetMessageName() == "GoingUp") {

		walkingright = false;
		walkingleft = false;
		walkingup = true;
		walkingdown = false;
		PlaySpriteAnimation(time, SAT_OneShot, 4, 6, "WalkingFront");

	}

	else if (message->GetMessageName() == "GoingLeft") {

		walkingright = false;
		walkingleft = true;
		walkingup = false;
		walkingdown = false;
		PlaySpriteAnimation(time, SAT_OneShot, 8, 11, "WalkingLeft");

	}

	else if (message->GetMessageName() == "GoingDown") {

		walkingright = false;
		walkingleft = false;
		walkingup = false;
		walkingdown = true;
		PlaySpriteAnimation(time, SAT_OneShot, 0, 3, "WalkingBack");

	}

}

void Pikachu::Render() {

	Actor::Render();

}
