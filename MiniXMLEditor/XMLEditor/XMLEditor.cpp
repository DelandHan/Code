// XMLEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WndBoard.h"

int main()
{
	WndBoard wnd;
	wnd.initialize();
	wnd.update();

	autownd::msgLoop();

    return 0;
}

