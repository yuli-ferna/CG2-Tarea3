#include "userInterface.h"
#include <iostream>
using namespace std;

extern unsigned int windowWidth, windowHeight;

// Global static pointer used to ensure a single instance of the class.
userInterface* userInterface::mInterface = NULL;

/**
* Creates an instance of the class
*
* @return the instance of this class
*/
userInterface* userInterface::Instance()
{
	if (!mInterface)   // Only allow one instance of class to be generated.
		mInterface = new userInterface();

	return mInterface;
}

userInterface::userInterface()
{
	mUserInterface = TwNewBar("Obj");

	TwDefine("Obj refresh = '0.0001f'");
	TwDefine("Obj resizable = false");
	TwDefine("Obj fontresizable = false");
	TwDefine("Obj movable = true");
	TwDefine("Obj visible = true");
	TwDefine("Obj position = '10 10'");
	TwDefine("Obj size = '250 200'");
	TwDefine("Obj color='143 66 244'");

	TwAddSeparator(mUserInterface, "sep1", NULL);

}

userInterface::~userInterface()
{
}


void userInterface::reshape()
{
	TwWindowSize(windowHeight, windowHeight);
}

void userInterface::show()
{
	TwDefine("Obj visible = true");
}

void userInterface::hide()
{
	TwDefine("Obj visible = false");
}
