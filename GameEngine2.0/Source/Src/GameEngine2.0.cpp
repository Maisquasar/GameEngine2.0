
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
	//_CrtSetBreakAlloc(593456);
	Main();
	return 0;
}

//TODO: Lights
//TODO: Fix Animation for other exportation than Mixamo
//TODO: Physics
//TODO: Add Icon Capsule Collide
//TODO: Show 3D models on Models selection (Same for Materials spheres)
//TODO: Fix Skeletal Model Node Creation