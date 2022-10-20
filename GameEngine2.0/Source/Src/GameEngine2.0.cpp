
#include <iostream>
#include "Include/App.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

void Main()
{
    App app = App("GameEngine2.0", 800, 600);
    app.InitializeApp();

    app.Update();

    app.ClearApp();
};

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(155);
    Main();
    _CrtDumpMemoryLeaks();
    return 0;
}
