#include "WndProgram.h"
#include <crtdbg.h>

using namespace autownd;
using namespace std;
using namespace memory;

namespace
{
	struct WndMsgSet
	{
		WndObj * _obj;
		const MsgSet * _set;
	};
}

IMsgProcess * MsgSet::retrieve(UINT msg) const
{
	map<UINT, IMsgProcess*>::const_iterator it = theMap.find(msg);
	if (it == theMap.end()) return nullptr;
	else return it->second;
}

void MsgSet::addMsgPair(UINT msg, IMsgProcess * proc)
{
	theMap.insert(pair<UINT, IMsgProcess*>(msg, proc));
}

////////////////////////////////////////////////

void Seed::init(memory::ParamChain params)
{
	static wchar_t clsname[3] = { 0,0,0 };
	(*((int*)clsname))++;
	theName = clsname;

	WNDCLASSEX wndclass;

	//wnd class
	ZeroMemory(&wndclass, sizeof(WNDCLASSEX));

	//fixed data
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = GetModuleHandle(0);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = theName.c_str();

	//custmized data
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	/*
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	*/

	RegisterClassEx(&wndclass);
}

int Seed::create(WndObj *obj, const MsgSet *msgmap, memory::ParamChain params)
{
	//no obj or obj already inited
	if (obj == nullptr || obj->theWnd != nullptr) return 1;
	//class name was not init;
	if (theName.size() == 0) return 1;

	//params
	const wchar_t * title = L"Title";
	vec size = { CW_USEDEFAULT , 0 }, pos = { CW_USEDEFAULT,0 };
	HWND parent = nullptr;
	long style = WS_OVERLAPPEDWINDOW;
	long exstyle = 0;
	int imenu = 0;

	//stream params
	find(params, "title", title);
	find(params, "size", size);
	find(params, "parent", parent);
	find(params, "style", style);
	find(params, "pos", pos);
	find(params, "exstyle", exstyle);
	find(params, "menu", imenu);

	//creating
	HMENU hmenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(imenu));

	WndMsgSet wmset = { obj,msgmap };
	CreateWindowEx(exstyle, theName.c_str(), title, style,
		pos.first, pos.second, size.first, size.second, parent, hmenu, GetModuleHandle(0), &wmset);

	DeleteObject(hmenu);

	if (obj->theWnd == nullptr) return GetLastError();
	else return 0;
}

LRESULT Seed::WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	const MsgSet * current = nullptr;

	if (msg == WM_NCCREATE)
	{
		WndMsgSet* pSetData = static_cast<WndMsgSet *>((reinterpret_cast<LPCREATESTRUCT>(lp))->lpCreateParams);

		pSetData->_obj->theWnd = wnd;
		SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pSetData->_set));

		current = pSetData->_set;
	}
	else
	{
		current = reinterpret_cast<MsgSet*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
		if (current == 0) return DefWindowProc(wnd, msg, wp, lp);
	}

	IMsgProcess * proc = current->retrieve(msg);
	if (proc == nullptr) return DefWindowProc(wnd, msg, wp, lp);
	if (proc->handleMsg(wp, lp) == 0) return 0;
	else return DefWindowProc(wnd, msg, wp, lp);
}

//////////////////////////

WndObj::WndObj()
	:theWnd(nullptr)
{
}

WndObj::~WndObj()
{
	SetWindowLongPtr(theWnd, GWLP_USERDATA, 0);
	DestroyWindow(theWnd);
}

int autownd::WndObj::addControl(WndObj * obj, TCHAR * cname, ParamChain params)
{
	//params
	const wchar_t * title = L"Title";
	vec size = { 200 , 600 }, pos = { 10,10 };
	long style = 0;
	long exstyle = 0;

	//stream params
	find(params, "title", title);
	find(params, "size", size);
	find(params, "pos", pos);
	find(params, "style", style);
	find(params, "exstyle", exstyle);

	style |= WS_CHILD;

	obj->theWnd = CreateWindowEx(exstyle, cname, title, style, pos.first, pos.second, size.first, size.second, theWnd, 0, GetModuleHandle(0), nullptr);

	if (obj->theWnd == nullptr) return GetLastError();
	else return 0;
}


int autownd::msgLoop(pair<HWND, int> acc)
{
	MSG msg;

	HACCEL hAccelTable = LoadAccelerators(GetModuleHandle(0), MAKEINTRESOURCE(acc.second));
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateAccelerator(acc.first == 0 ? msg.hwnd : acc.first, hAccelTable, &msg);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(hAccelTable);

	return msg.lParam;
}

