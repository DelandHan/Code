#pragma once
#include "IModule.h"
class ExpWnd
{
public:
	ExpWnd();
	~ExpWnd();

	void init(IModule * module);

	//update the left panel
	void updateItemlist(LPARAM param);
	//update the right panel
	void updateChildList(LPARAM param);
	//update the right panel
	void updateAttlist(LPARAM param);

	//handle the WM_Notify message of childs
	int beNotified(memory::ParamChain params);
	//handle button message
	int clickButton(memory::ParamChain params);

	//use to receive the edit result of the right panel
	int setAttribute(memory::ParamChain params);

	//reset the layout
	int updateLayout(memory::ParamChain params);

	inline HWND wnd() { return theMainWnd.wnd(); }

private:
	autownd::WndObj theMainWnd;
	autownd::FileDialog theOpenDialog;

	class ExpPanel
	{
	public:
		ExpPanel();
		~ExpPanel();

		void setRect(autownd::vec size, autownd::vec pos);
		void init(autownd::WndObj *parent, int type);
		void move(const RECT * clientRect);
		void clear();
		inline HWND wnd() { return theObj.wnd(); }
		inline LPARAM param() { return theParam; }

		//update the listview
		void update(memory::BulletChain * chain);

		autownd::List theObj;

	private:
		LPARAM theParam;

		autownd::vec theSize, thePos;
		int theType;
	}theLeftPanel, theRightPanel, theAttPanel; //item list
	
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
		LPARAM param; //the para of the item that is right-clicked on.
	}theContext;
	
	IModule * theData;
};

