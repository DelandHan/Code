#pragma once
#include "IModule.h"
class ExpWnd
{
public:
	ExpWnd();
	~ExpWnd();

	void init(IModule * module);

	//update the left panel
	void updateItemlist(LPARAM param = 0);
	//update the right panel
	void updateAttlist(LPARAM param = 0);

	//handle the WM_Notify message of childs
	void beNotified(LPNMHDR data);
	//handle button message
	void clickButton(HWND wnd, int msg);

	//use to receive the edit result of the right panel
	void setAttribute(TCHAR * buff);

	int updateLayout(autownd::WndObj *obj, WPARAM wp, LPARAM lp);
private:
	autownd::WndObj theMainWnd;

	struct
	{
		autownd::List obj;
		LPARAM param;
	}theLeftPanel;

	struct
	{
		autownd::List obj;
		LPARAM param;
	}theRightPanel;
	
	struct
	{
		autownd::Edit obj;
		std::wstring str[2];
		size_t subitem;
	} theEdit;

	struct
	{
		autownd::WndObj obj; 
		LPARAM param;
	}theButton;
		

	IModule * theData;
};

