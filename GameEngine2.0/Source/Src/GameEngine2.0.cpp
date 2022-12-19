
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
    //  TODO: Remove Comments
    // |
    // V
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // A faire une fois au début de l'application
    //_CrtSetBreakAlloc(1014553);
    Main();
    return 0;
}

//TODO : 
//		- Fix ofbx.h leaks.
//		- Clean Loader Code.
//      - Fix Bone Loading (dans le .scene specifier si c'est un bone et mettre l'id et le default transform)
//      - Fix Bone Delete.