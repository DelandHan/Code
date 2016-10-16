// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DisplayHub.h"
#include "CenterUnit.h"
#include "XMLDataHub.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	XMLDataHub xmldata;
	xmldata.loadFile("IECU.xml");

	CenterUnit cunit;
	InputUnit input;

	DisplayHub ui;
	ui.initialize();

	cunit.connect(&ui, &xmldata);
	ui.connectToInputHub(&input);
	input.connectTo(&cunit);

	input.dbClick(0);

	autownd::msgLoop();
    return 0;
}

