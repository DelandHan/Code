// MiniXMLEditor.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "ExpWnd.h"
#include "XMLPool.h"
#include "resource.h"

using namespace std;
using namespace autownd;


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ExpWnd exp;
	XMLPool ps;
	exp.init(&ps);

	return autownd::msgLoop(std::pair<HWND,int>(exp.wnd(),IDR_ACCELERATOR1));

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}
