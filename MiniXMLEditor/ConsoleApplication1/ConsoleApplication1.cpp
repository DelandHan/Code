// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DisplayHub.h"
#include "XMLDataHub.h"
#include "resource.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	XMLDataHub xmldata;
	xmldata.loadFile("IECU.xml");

	DisplayHub ui;

	ui.initialize();
	ui.connectToDataPool(&xmldata);

	
	autownd::msgLoop({ ui.wnd(), IDR_ACCELERATOR1 });
    return 0;
}

