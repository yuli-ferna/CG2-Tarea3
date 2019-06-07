#pragma once
#include <AntTweakBar.h>
#include <iostream>
#include <string>

class userInterface {
public:
	static userInterface* mInterface; //Holds the instance of the class
	TwBar* mUserInterface;
	///Method to obtain the only instance of the calls
	static userInterface* Instance();
	~userInterface(); 
	void reshape();
	void show();
	void hide();
private:
	userInterface();

};