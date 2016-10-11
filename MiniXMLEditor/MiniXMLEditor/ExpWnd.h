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
	int beNotified(memory::ParamChain params);
	//handle button message
	int clickButton(memory::ParamChain params);

	//use to receive the edit result of the right panel
	int setAttribute(memory::ParamChain params);

	//reset the layout
	int updateLayout(memory::ParamChain params);

private:
	autownd::WndObj theMainWnd;
	autownd::FileDialog theOpenDialog;

	struct
	{
		autownd::List obj;
		LPARAM param;
	}theLeftPanel; //item list

	struct
	{
		autownd::List obj;
		LPARAM param;
	}theRightPanel; //childs list

	struct
	{
		autownd::List obj;
		LPARAM param;
	}theAttPanel; //att list
	
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

	autownd::WndObj thePath;
	
	struct
	{
		autownd::ContextMenu obj;
		LPARAM param;
	}theContext;
	
	IModule * theData;
};

