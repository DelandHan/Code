// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DisplayHub.h"
#include "XMLDataHub.h"
#include "resource.h"



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	XMLDataHub xmldata;
	DisplayHub ui;

	ui.initialize();
	ui.connectToDataPool(&xmldata);

	
	autownd::msgLoop({ ui.wnd(), IDR_ACCELERATOR1 });
    return 0;
}


int _stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	return main();
}