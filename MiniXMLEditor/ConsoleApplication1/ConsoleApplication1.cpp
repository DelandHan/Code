// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DisplayHub.h"
#include "XMLDataHub.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	XMLDataHub xmldata;
	xmldata.loadFile("IECU.xml");

	DisplayHub ui;

	ui.initialize();
	ui.connectToDataPool(&xmldata);

	
	autownd::msgLoop();
    return 0;
}

