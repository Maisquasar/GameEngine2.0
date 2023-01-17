
#include <iostream>
#include "Include/App.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

void Main()
{
	Application.InitializeApp("GameEngine2.0", 1920, 1080);

	Application.Update();

	Application.ClearApp();
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//  TODO: Remove Comments
	// |
	// V
	//_CrtSetBreakAlloc(1865684);
	Main();
	return 0;
}

//TODO: Lights
//TODO: Loading Texture from FBX files
//TODO: Fix Animation for other exportation than Mixamo
//TODO: Physics