
#include <iostream>
#include "Include/App.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

void Main()
{
    App app = App("GameEngine2.0", 1920, 1080);
    app.InitializeApp();

    app.Update();

    app.ClearApp();
};

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // A faire une fois au d�but de l'application
    //_CrtSetBreakAlloc(110582);
    Main();
    return 0;
}
